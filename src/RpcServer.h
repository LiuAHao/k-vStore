#include <string>

class RpcServer {
public:
    RpcServer(const std::string& address, const std::string& port);
    ~RpcServer();
};