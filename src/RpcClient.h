#include <string>

class RpcClient {
public:
    RpcClient(const std::string& address, const std::string& port);
    ~RpcClient();
};