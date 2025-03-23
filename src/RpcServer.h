#pragma once
#include <string>
#include <functional>

// 定义请求投票处理器类型
typedef std::function<bool(int64_t term, int64_t candidateId)> RequestVoteHandler;

class RpcServer 
{
private:
    std::string address;
    std::string port;
    RequestVoteHandler voteHandler;

public:
    RpcServer(const std::string& address, const std::string& port);
    ~RpcServer();
    void registerHandler(RequestVoteHandler handler);
    void startListening(int port);
};