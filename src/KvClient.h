#pragma once
#include <string>
#include <vector>

class KvClient 
{
private:
    std::string currentAddress;
    std::vector<std::string> servers;
    size_t currentServerIndex;
    
public:
    KvClient(std::vector<std::string>& servers);
    ~KvClient();
    
    bool set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    bool deleteKey(const std::string& key);
    void rotateServer();    // 切换到下一个服务器
};