#include <string>

class RaftNode {
public:
    RaftNode(const std::string& address, const std::string& port);
    ~RaftNode();
};