#include "RpcServer.h"

RpcServer::RpcServer(const std::string& address, const std::string& port)
    : address(address), port(port) {
    // 初始化 RPC 服务器

