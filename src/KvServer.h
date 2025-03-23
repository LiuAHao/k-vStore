#include <string>
#include "RaftNode.h"
#include "KvStateMachine.h"

class KvServer {
private:
    RaftNode& raftNode;
    KvStateMachine stateMachine;

public:
    KvServer(const std::string& address, const std::string& port);
    ~KvServer();
    
    std::string get(const std::string& key);
    void set(const std::string& key, const std::string& value);
    void deleteKey(const std::string& key);

        
};