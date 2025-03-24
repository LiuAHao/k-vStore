#include "KvServer.h"
#include <iostream>
#include <future>

KvServer::KvServer(int nodeId, const std::string& configPath) 
    : stateMachine("/tmp/kvstore_" + std::to_string(nodeId)) {
    
    // 将回调方法移到public部分后，这里可以正常调用
    raftNode->setLogAppliedCallback([this](uint64_t index, const LogEntry& entry) {
        onLogApplied(index, entry);
    });
    
    raftNode->setLeadershipChangeCallback([this](bool isLeader) {
        onLeadershipChange(isLeader);
    });
}

void KvServer::onLogApplied(uint64_t index, const LogEntry& entry) {
    std::lock_guard<std::mutex> lock(pendingReqMutex);
    if (auto it = pendingRequests.find(entry.getRequestId()); it != pendingRequests.end()) {
        it->second.set_value(true);  // 修改为直接设置bool值
        pendingRequests.erase(it);
    }
}

std::future<bool> KvServer::asyncSet(const std::string& key, const std::string& value) {
    std::promise<bool> promise;
    auto future = promise.get_future();
    
    if (!raftNode->isLeader()) {
        promise.set_exception(std::make_exception_ptr(
            std::runtime_error("Not leader")
        ));
        return future;
    }

    static std::atomic<int64_t> requestIdCounter{0};
    int64_t reqId = ++requestIdCounter;

    LogEntry entry(LogEntry::Type::SET, key, value, reqId, 0, 0);
    
    {
        std::lock_guard<std::mutex> lock(pendingReqMutex);
        pendingRequests.emplace(entry.getRequestId(), std::move(promise));
    }
    
    try {
        raftNode->propose(entry);
    } catch (const std::exception& e) {
        pendingRequests.erase(entry.getRequestId());
        throw;
    }
    
    return future;
}

// 触发快照
void KvServer::triggerSnapshot() {
    if (raftNode->isLeader()) {
        std::string snapshotPath = "/tmp/snapshot_" + std::to_string(time(nullptr));
        if (stateMachine.createSnapshot(snapshotPath)) {
            // 处理快照创建成功
            std::cout << "快照创建成功: " << snapshotPath << std::endl;
        }
    }
}
