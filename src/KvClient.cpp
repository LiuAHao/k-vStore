#include <KvClient.h>

KvClient::KvClient(std::vector<std::string> &serverAddrs):servers(serverAddrs)
{
    servers = serverAddrs;
    creds = grpc::InsecureChannelCredentials();     // 初始化 gRPC 凭证
    currentLeader = servers[0];     //猜测第一个是leader
}

bool KvClient::set(std::string &key, std::string &value)
{
    while (true)
    {
        auto channel = grpc::CreateChannel(currentLeader, creds);   // 创建 gRPC 通道连接当前的 leader
        auto stub = KvService::NewStub(channel);    // 创建 gRPC 存根

        ClientRequest request;
        request.set_key(key);
        request.set_value(value);
        request.set_op(SET);

        ClientResponse resp;
        grpc::ClientContext context;    // 用于存储 gRPC 上下文信息
        auto status = stub->ClientCommand(&context, request, &resp);    // 发送 gRPC 请求并获取响应

        if (status.ok())    // 检查 gRPC 响应状态
        {
            if(status.ok())
            {
                return true;
            }
            else if(resp.error() == NOT_LEADER)
            {
                currentLeader = resp.leader_hint();     // 更新当前的 leader
                continue;
            }
        }
        else
        {
            //网络故障，尝试其他节点
            rotateServer();
        }
    }
}

void KvClient::rotateServer()
{
    std::rotate(servers.begin(), servers.begin() + 1, servers.end());       // 旋转服务器列表,将第一个元素移动到最后
    currentLeader = servers[0];     // 更新当前的 leader
}




