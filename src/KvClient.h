#pragma once
#include <string>
#include <vector>
#include <memory>
#include <future>
#include <grpcpp/grpcpp.h>
#include "kvstore.grpc.pb.h"

class KvClient {
private:
    struct Impl; // PIMPL 模式隐藏实现细节
    std::unique_ptr<Impl> impl;

public:
    explicit KvClient(const std::vector<std::string>& servers);
    ~KvClient();

    // 同步接口
    bool set(const std::string& key, const std::string& value, int retry = 3);
    std::string get(const std::string& key, bool linearizable = true);
    bool deleteKey(const std::string& key);
    
    // 异步接口
    std::future<bool> asyncSet(const std::string& key, const std::string& value);
    std::future<std::string> asyncGet(const std::string& key);

    // 集群状态查询
    std::string getLeaderAddress() const;
    std::vector<std::string> getAllServers() const;
    
    // 修改获取当前领导者方法
    std::string getCurrentLeader() const { return getLeaderAddress(); }
};