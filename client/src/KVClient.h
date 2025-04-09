#include <grpcpp/grpcpp.h>
#include "../../kvstore.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using kvstore::KVStore;  // 添加这行
using kvstore::MyRequest;  // 添加这行
using kvstore::MyResponse;  // 添加这行

class KVClient {
public:
    KVClient(std::shared_ptr<Channel> channel);

    std::string CallMethod(const std::string& name);

private:
    std::unique_ptr<KVStore::Stub> stub_;  // 修改为KVStore::Stub
};

