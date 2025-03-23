#include "KvClient.h"
#include <iostream>

KvClient::KvClient(std::vector<std::string>& servers) 
    : servers(servers), currentServerIndex(0) {
    
    if (servers.empty()) {
        std::cerr << "错误：服务器列表为空" << std::endl;
        return;
    }
    
    currentAddress = servers[currentServerIndex];
    std::cout << "KvClient 初始化，连接到 " << currentAddress << std::endl;
    
    // 在实际的gRPC实现中，这里会创建通道和存根
    // 现在我们只是模拟
}

KvClient::~KvClient() {
    // 清理资源
}

bool KvClient::set(const std::string& key, const std::string& value) {
    std::cout << "发送SET请求: key=" << key << ", value=" << value << " 到 " << currentAddress << std::endl;
    
    // 在实际实现中，这里会通过gRPC发送请求
    // 现在我们只是模拟成功
    return true;
}

std::string KvClient::get(const std::string& key) {
    std::cout << "发送GET请求: key=" << key << " 到 " << currentAddress << std::endl;
    
    // 在实际实现中，这里会通过gRPC发送请求
    // 现在我们只是模拟返回一个值
    return "模拟的值_" + key;
}

bool KvClient::deleteKey(const std::string& key) {
    std::cout << "发送DELETE请求: key=" << key << " 到 " << currentAddress << std::endl;
    
    // 在实际实现中，这里会通过gRPC发送请求
    // 现在我们只是模拟成功
    return true;
}

void KvClient::rotateServer() {
    currentServerIndex = (currentServerIndex + 1) % servers.size();
    currentAddress = servers[currentServerIndex];
    std::cout << "切换到服务器: " << currentAddress << std::endl;
}




