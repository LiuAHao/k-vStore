#include <iostream>
#include <vector>
#include <string>
#include "KvServer.h"
#include "KvClient.h"

int main(int argc, char* argv[]) {
    std::cout << "键值存储系统启动..." << std::endl;
    
    // 创建一个服务器实例
    KvServer server("127.0.0.1", "50051");
    
    // 测试服务器直接操作
    server.set("test_key", "test_value");
    std::string value = server.get("test_key");
    std::cout << "服务器直接获取: key=test_key, value=" << value << std::endl;
    
    // 创建一个客户端
    std::vector<std::string> serverAddresses = {"127.0.0.1:50051"};
    KvClient client(serverAddresses);
    
    // 测试客户端操作
    client.set("client_key", "client_value");
    std::string clientValue = client.get("client_key");
    std::cout << "客户端获取: key=client_key, value=" << clientValue << std::endl;
    
    // 测试删除操作
    client.deleteKey("client_key");
    
    std::cout << "测试完成" << std::endl;
    
    return 0;
}