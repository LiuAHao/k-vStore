#include "RpcServer.h"
#include <iostream>

RpcServer::RpcServer(const std::string& address, const std::string& port)
    : address(address), port(port) {
    // 初始化 RPC 服务器
    std::cout << "RPC服务器初始化在 " << address << ":" << port << std::endl;
}

RpcServer::~RpcServer() {
    // 清理资源
    std::cout << "RPC服务器关闭" << std::endl;
}

void RpcServer::registerHandler(RequestVoteHandler handler) {
    voteHandler = handler;
    std::cout << "注册了请求投票处理器" << std::endl;
}

void RpcServer::startListening(int port) {
    std::cout << "RPC服务器开始监听端口: " << port << std::endl;
    // 在实际实现中，这里会启动gRPC服务器
}

