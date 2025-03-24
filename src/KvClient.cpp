#include "KvClient.h"
#include <algorithm>
#include <thread>
#include "RaftNode.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::CreateChannel;
using grpc::InsecureChannelCredentials;

class KvClient::Impl {
private:
    std::vector<std::unique_ptr<kvstore::KvStore::Stub>> stubs;
    mutable int currentLeader;
    mutable std::mutex leaderMutex;

protected:
    template<typename Req, typename Resp>
    bool rpcWithRetry(int methodType, Req& req, Resp& resp, int retry) {
        constexpr int maxRetry = 5;
        int attempts = 0;
        
        while (attempts++ < maxRetry) {
            {
                std::lock_guard<std::mutex> lock(leaderMutex);
                if (currentLeader >= stubs.size()) currentLeader = 0;
            }
            
            ClientContext ctx;
            ctx.set_deadline(std::chrono::system_clock::now() + 
                           std::chrono::milliseconds(500));
            
            Status status;
            switch(methodType) {
                case 0: 
                    status = stubs[currentLeader]->ClientCommand(&ctx, req, &resp);
                    break;
                default:
                    return false;
            }
            
            if (status.ok()) {
                if (resp.error_code() == kvstore::ErrorCode::NOT_LEADER) {
                    updateLeader(resp.leader_hint());
                    continue;
                }
                return resp.success();
            }
            
            rotateLeader();
        }
        return false;
    }

public:
    Impl(const std::vector<std::string>& servers) : currentLeader(0) {
        for (const auto& addr : servers) {
            auto channel = CreateChannel(addr, InsecureChannelCredentials());
            stubs.push_back(kvstore::KvStore::NewStub(channel));
        }
    }

    bool set(const std::string& key, const std::string& value, int retry = 3) {
        kvstore::CommandRequest req;
        req.set_key(key);
        req.set_value(value);
        req.set_operation_type(kvstore::OperationType::SET);
        
        kvstore::CommandResponse resp;
        return rpcWithRetry(0, req, resp, retry);
    }

    std::string get(const std::string& key, bool linearizable) {
        kvstore::CommandRequest req;
        req.set_key(key);
        req.set_operation_type(kvstore::OperationType::GET);
        
        kvstore::CommandResponse resp;
        if (rpcWithRetry(0, req, resp, 3)) {
            return resp.value();
        }
        return "";
    }

    bool del(const std::string& key, int retry = 3) {
        kvstore::CommandRequest req;
        req.set_key(key);
        req.set_operation_type(kvstore::OperationType::DELETE);
        
        kvstore::CommandResponse resp;
        return rpcWithRetry(0, req, resp, retry);
    }
};

// 客户端接口实现
KvClient::KvClient(const std::vector<std::string>& servers) 
    : impl(std::make_unique<Impl>(servers)) {}

KvClient::~KvClient() = default;

std::string KvClient::get(const std::string& key, bool linearizable) {
    return impl->get(key, linearizable);
}

bool KvClient::set(const std::string& key, const std::string& value, int retry) {
    return impl->set(key, value, retry);
}

bool KvClient::del(const std::string& key, int retry) {
    return impl->del(key, retry);
}

std::string KvClient::getCurrentLeader() const {
    return impl->getCurrentLeader();
}




