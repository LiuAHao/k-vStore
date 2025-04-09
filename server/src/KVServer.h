#include <grpcpp/grpcpp.h>
#include "../../kvstore.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using kvstore::KVStore;
using kvstore::MyRequest;
using kvstore::MyResponse;

class KVServiceImpl final : public KVStore::Service {
    Status MyMethod(ServerContext* context, const MyRequest* request, MyResponse* reply) override;
};

