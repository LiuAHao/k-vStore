#include "KvServer.h"

KvServer::KvServer(const std::string& address, const std::string& port)
    : address(address), port(port) {
    // 初始化服务器
    // 创建服务器实例
    server = new grpc::ServerBuilder();
   
}

