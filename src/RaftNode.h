#pragma once
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <random>
#include "LogEntry.h"
#include "PersistentStorage.h"
#include "KvStateMachine.h"

// Raft 节点状态枚举
enum class NodeState {
    FOLLOWER,
    CANDIDATE,
    LEADER
};

// 集群节点信息
struct NodeInfo {
    int id;
    std::string address;
    std::string port;
};

class RaftNode {
public:
    // 基本信息和状态
    int nodeId;
    std::vector<NodeInfo> peers;
    std::string address;
    std::string port;
    
    // Raft状态
    std::mutex stateMutex;
    NodeState state;
    int64_t currentTerm;
    int votedFor;
    std::vector<LogEntry> log;
    
    // 易失性状态
    int64_t commitIndex;
    int64_t lastApplied;
    std::map<int, int64_t> nextIndex;
    std::map<int, int64_t> matchIndex;
    
    // 组件
    PersistentStorage storage;
    KvStateMachine* stateMachine;
    
    // 选举相关
    std::atomic<bool> running;
    std::thread electionThread;
    std::thread applyThread;
    std::mutex electionMutex;
    std::condition_variable electionCV;
    std::mt19937 rng;
    std::uniform_int_distribution<int> electionTimeoutDist;
    
    // 新增：心跳时间戳
    int64_t lastHeartbeatTime;

public:
    // 构造函数和析构函数
    RaftNode(int nodeId, const std::string& address, const std::string& port, 
             const std::vector<NodeInfo>& peers, KvStateMachine* stateMachine);
    ~RaftNode();

    // 公共接口
    void start();
    void stop();
    bool isLeader() const;
    std::string getLeaderAddress() const;
    int64_t getLastCommittedIndex() const;
    int64_t propose(const LogEntry& entry);
    
    // 成员变更
    bool addServer(const NodeInfo& newNode);
    bool removeServer(int nodeId);

    // 添加回调方法到public部分
    void setLogAppliedCallback(std::function<void(uint64_t, const LogEntry&)> callback) {
        logAppliedCallback = callback;
    }
    void setLeadershipChangeCallback(std::function<void(bool)> callback) {
        leadershipChangeCallback = callback;
    }

private:
    // 内部方法
    void becomeFollower(int64_t term);
    void becomeCandidate();
    void becomeLeader();
    void sendHeartbeats();
    void applyCommittedEntries();
    void resetElectionTimer();
    bool isLogUpToDate(int64_t lastLogIndex, int64_t lastLogTerm);
    
    // RPC处理方法
    bool handleRequestVote(int64_t term, int candidateId, int64_t lastLogIndex, 
                         int64_t lastLogTerm, bool& voteGranted);
    bool handleAppendEntries(int64_t term, int leaderId, int64_t prevLogIndex,
                           int64_t prevLogTerm, const std::vector<LogEntry>& entries,
                           int64_t leaderCommit, bool& success);

    // 添加回调函数成员
    std::function<void(uint64_t, const LogEntry&)> logAppliedCallback;
    std::function<void(bool)> leadershipChangeCallback;
    
    // 添加 RPC 客户端
    class RpcClient {
    public:
        void sendAppendEntries(int peerId, int64_t term, int nodeId, 
                             int64_t prevLogIndex, int64_t prevLogTerm,
                             const std::vector<LogEntry>& entries, int64_t commitIndex,
                             std::function<void(int, int64_t, bool)> callback) {
            // TODO: 实现真实的RPC调用
            callback(peerId, term, true);  // 临时模拟成功响应
        }
    };
    
    RpcClient rpcClient;
};