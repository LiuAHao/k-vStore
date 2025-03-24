#pragma once
#include <string>
#include <mutex>
#include <map>
#include <future>
#include "RaftNode.h"
#include "KvStateMachine.h"

class KvServer {
private:
    std::unique_ptr<RaftNode> raftNode;
    KvStateMachine stateMachine;
    std::mutex pendingReqMutex;
    std::map<int64_t, std::promise<bool>> pendingRequests;

public:
    // 添加构造函数声明
    KvServer(int nodeId, const std::string& configPath);
    ~KvServer() = default;

    // 添加方法声明
    void onLogApplied(uint64_t index, const LogEntry& entry);
    void onLeadershipChange(bool isLeader);
    std::future<bool> asyncSet(const std::string& key, const std::string& value);
    void triggerSnapshot();
};