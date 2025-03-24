#pragma once
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <grpcpp/grpcpp.h>
#include "protos/kvstore.grpc.pb.h"

class KvClient {
private:
    class Impl;
    std::unique_ptr<Impl> impl;

public:
    // 构造函数
    KvClient(const std::vector<std::string>& servers);
    ~KvClient();
    
    // 客户端接口
    std::string get(const std::string& key, bool linearizable = false);
    bool set(const std::string& key, const std::string& value, int retry = 3);
    bool del(const std::string& key, int retry = 3);
    std::string getCurrentLeader() const;
};