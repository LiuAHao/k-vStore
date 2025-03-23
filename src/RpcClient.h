#pragma once
#include <string>
#include "RaftNode.h"

class RpcClient 
{
private:
public:
    RpcClient();
    RpcClient(const std::string& address, const std::string& port);
    ~RpcClient();
    void sendRequestVote(RaftNode* target);     // 发送请求投票请求
    void sendAppendEntries(RaftNode* target);   // 发送心跳请求
};