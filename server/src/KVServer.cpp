#include "KVServer.h"

Status KVServiceImpl::MyMethod(ServerContext* context, const MyRequest* request, MyResponse* reply) {
    (void)context;  // 显式标记未使用
    std::string prefix("Hello ");
    reply->set_message(prefix + request->name());
    return Status::OK;
}

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    KVServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char** argv) {
    (void)argc; (void)argv;  // 显式标记未使用
    RunServer();
    return 0;
}