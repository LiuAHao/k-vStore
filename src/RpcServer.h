#pragma once
#include <string>

class RpcServer 
{

public:
    RpcServer();
    ~RpcServer();
    void registerHandler(RequestVoteHandler handler);
    void startListening(int port);
};