#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "KvServer.h"
#include "KvClient.h"

void runServer(int nodeId, const std::string& configPath) {
    try {
        KvServer server(nodeId, configPath);
        std::cout << "节点 " << nodeId << " 已启动" << std::endl;
        std::this_thread::sleep_for(std::chrono::hours(1)); // 保持运行
    } catch (const std::exception& e) {
        std::cerr << "节点 " << nodeId << " 启动失败: " << e.what() << std::endl;
    }
}

void testCluster(const std::vector<std::string>& addresses) {
    KvClient client(addresses);
    
    // 测试基本操作
    client.set("name", "Alice");
    std::cout << "[测试1] 设置 name=Alice" << std::endl;
    
    std::cout << "[测试2] 读取 name: " << client.get("name") << std::endl;
    
    // 测试领导权变更
    std::cout << "\n--- 模拟领导者故障 ---" << std::endl;
    std::string leader = client.getCurrentLeader();
    std::cout << "当前领导者: " << leader << std::endl;
    std::cout << "终止领导者节点..." << std::endl;
    
    // 等待新领导者选举
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    client.set("name", "Bob");
    std::cout << "\n[测试3] 新领导者设置 name=Bob" << std::endl;
    std::cout << "当前值: " << client.get("name") << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc > 1) { // 节点启动模式
        int nodeId = std::stoi(argv[1]);
        runServer(nodeId, "cluster.conf");
        return 0;
    }

    // 启动3个节点集群
    std::vector<std::thread> nodes;
    for (int i = 1; i <= 3; ++i) {
        nodes.emplace_back([](int id) {
            system(("./kvstore " + std::to_string(id)).c_str());
        }, i);
    }
    
    // 等待集群初始化
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    // 运行测试用例
    testCluster({"127.0.0.1:50051", "127.0.0.1:50052", "127.0.0.1:50053"});

    // 清理
    for (auto& t : nodes) t.join();
    return 0;
}