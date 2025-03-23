#include "KvServer.h"

KvServer::KvServer(const std::string& address, const std::string& port)
    : address(address), port(port) {
    // 创建服务器实例
    server = new grpc::ServerBuilder();
   
}


KvServer::~KvServer() 
{
    // 清理资源
    delete server;
}


bool ok()
{

}
