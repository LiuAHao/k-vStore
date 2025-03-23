#include "KvServer.h"
#include <iostream>

KvServer::KvServer(const std::string& address, const std::string& port)
    : address(address), port(port) {
    // 初始化服务器
    std::cout << "KvServer 初始化在 " << address << ":" << port << std::endl;
}

KvServer::~KvServer() {
    // 清理资源
}

std::string KvServer::get(const std::string& key) {
    // 直接从状态机获取数据
    return stateMachine.get(key);
}

bool KvServer::set(const std::string& key, const std::string& value) {
    // 创建一个SET类型的日志条目
    // 注意：在完整的Raft实现中，这里应该将日志提交给Raft节点
    // 现在我们直接应用到状态机
    static int64_t requestId = 0;
    static int64_t term = 1;
    static int64_t index = 0;
    
    LogEntry entry(LogEntry::Type::SET, key, value, requestId++, term, index++);
    stateMachine.apply(entry);
    
    return true;
}

bool KvServer::deleteKey(const std::string& key) {
    // 创建一个DELETE类型的日志条目
    static int64_t requestId = 1000; // 使用不同的起始ID避免冲突
    static int64_t term = 1;
    static int64_t index = 1000;
    
    LogEntry entry(LogEntry::Type::DELETE, key, "", requestId++, term, index++);
    stateMachine.apply(entry);
    
    return true;
}
