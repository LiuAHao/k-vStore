#pragma once
#include <string>
#include <mutex>
#include <map>
#include "KvStateMachine.h"

class KvServer 
{
private:
    std::string address;
    std::string port;
    KvStateMachine stateMachine;    // 键值存储状态机
    std::mutex pendingReqMutex;     // 用于保护 pendingRequests 的互斥锁
    std::map<int, std::string> pendingRequests;     // 存储未完成的请求

public:
    KvServer(const std::string& address, const std::string& port);
    ~KvServer();
    
    std::string get(const std::string& key);
    bool set(const std::string& key, const std::string& value);
    bool deleteKey(const std::string& key);
};