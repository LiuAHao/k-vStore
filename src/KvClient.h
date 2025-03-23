#pragma once
#include <string>
#include <vector>
#include <grpcpp/grpcpp.h>
#include "ClientRequest.h"
#include "ClientResponse.h"

class KvClient 
{
private:
    std::string surrentAddress;
    std::vector<std::string> servers;
    grpc::ChannelCredentials* creds;    // 用于存储 gRPC 通道的凭据

public:
    KvClient(std::vector<std::string> &servers);
    ~KvClient();
    bool set(const std::string& key, const std::string& value);
    void rotateServer();    // 切换到下一个服务器
};