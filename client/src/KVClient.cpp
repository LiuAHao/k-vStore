#include "KVClient.h"

KVClient::KVClient(std::shared_ptr<Channel> channel)
    : stub_(KVStore::NewStub(channel)) {}  // 修改为KVStore::NewStub

std::string KVClient::CallMethod(const std::string& name) {
    MyRequest request;
    request.set_name(name);
    
    MyResponse reply;
    ClientContext context;
    
    Status status = stub_->MyMethod(&context, request, &reply);
    
    if (status.ok()) {
        return reply.message();
    } else {
        return "RPC failed";
    }
}

int main(int argc, char** argv) {
    (void)argc; (void)argv;  // 消除未使用参数警告
    std::string target_str("localhost:50051");
    KVClient client(
    grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
        
    std::string name("world");
    std::string reply = client.CallMethod(name);
    std::cout << "Received: " << reply << std::endl;
    
    return 0;
}