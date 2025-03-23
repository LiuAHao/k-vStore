#pragma once
#include <string>
#include <mutex>
#include <map>
#include "RaftNode.h"
#include "KvStateMachine.h"


class KvServer 
{
private:
    RaftNode& raftNode;             //关联的Raft节点
    KvStateMachine stateMachine;    // 键值存储状态机
    std::mutex pendingReqMutex;     // 用于保护 pendingRequests 的互斥锁
    std::map<int, std::string> pendingRequests;     // 存储未完成的请求

public:
    KvServer();
    ~KvServer();
    
    std::string get(const std::string& key);
    void set(const std::string& key, const std::string& value);
    void deleteKey(const std::string& key);
    bool ok();
    NewStub()

        
};