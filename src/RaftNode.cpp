#include "RaftNode.h"
#include <iostream>
#include <chrono>
#include <algorithm>

RaftNode::RaftNode(int nodeId, const std::string& address, const std::string& port, 
                  const std::vector<NodeInfo>& peers, KvStateMachine* stateMachine)
    : nodeId(nodeId), address(address), port(port), peers(peers), stateMachine(stateMachine),
      state(NodeState::FOLLOWER), currentTerm(0), votedFor(-1), commitIndex(0), lastApplied(0),
      running(false), storage("/tmp/raft_" + std::to_string(nodeId)),
      rng(std::random_device{}()),
      electionTimeoutDist(300, 500) { // 选举超时范围：300-500ms
    
    // 从持久化存储加载状态
    currentTerm = storage.loadTerm();
    votedFor = storage.loadVote();
    log = storage.loadLogs();
    
    // 初始化 nextIndex 和 matchIndex
    for (const auto& peer : peers) {
        if (peer.id != nodeId) {
            nextIndex[peer.id] = 1;
            matchIndex[peer.id] = 0;
        }
    }
}

RaftNode::~RaftNode() {
    stop();
}

void RaftNode::start() {
    std::lock_guard<std::mutex> lock(stateMutex);
    if (running) return;
    
    running = true;
    
    // 启动选举线程
    electionThread = std::thread([this]() {
        while (running) {
            std::unique_lock<std::mutex> lock(electionMutex);
            auto timeout = std::chrono::milliseconds(electionTimeoutDist(rng));
            
            // 等待超时或者被通知
            if (electionCV.wait_for(lock, timeout, [this]() { return !running; })) {
                // 如果被通知停止，则退出
                break;
            }
            
            // 检查是否需要发起选举
            std::lock_guard<std::mutex> stateLock(stateMutex);
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                now - std::chrono::steady_clock::time_point(
                    std::chrono::steady_clock::duration(lastHeartbeatTime))).count();
            
            if (state != NodeState::LEADER && elapsed >= electionTimeoutDist(rng)) {
                becomeCandidate();
            }
        }
    });
    
    // 启动日志应用线程
    applyThread = std::thread([this]() {
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            applyCommittedEntries();
        }
    });
    
    std::cout << "Raft 节点 " << nodeId << " 启动在 " << address << ":" << port << std::endl;
}

void RaftNode::stop() {
    {
        std::lock_guard<std::mutex> lock(stateMutex);
        if (!running) return;
        running = false;
    }
    
    // 通知选举线程退出
    electionCV.notify_all();
    
    // 等待线程结束
    if (electionThread.joinable()) {
        electionThread.join();
    }
    
    if (applyThread.joinable()) {
        applyThread.join();
    }
    
    std::cout << "Raft 节点 " << nodeId << " 已停止" << std::endl;
}

void RaftNode::resetElectionTimer() {
    lastHeartbeatTime = std::chrono::steady_clock::now().time_since_epoch().count();
}

void RaftNode::becomeFollower(int64_t term) {
    state = NodeState::FOLLOWER;
    currentTerm = term;
    votedFor = -1;
    
    // 持久化状态
    storage.saveTerm(currentTerm);
    storage.saveVote(votedFor);
    
    resetElectionTimer();
    std::cout << "节点 " << nodeId << " 成为 Follower，任期 " << currentTerm << std::endl;
}

void RaftNode::becomeCandidate() {
    state = NodeState::CANDIDATE;
    currentTerm++;
    votedFor = nodeId;
    
    storage.saveTerm(currentTerm);
    storage.saveVote(votedFor);
    
    resetElectionTimer();
    std::cout << "节点 " << nodeId << " 开始选举，任期 " << currentTerm << std::endl;
}

void RaftNode::sendHeartbeats() {
    std::lock_guard<std::mutex> lock(stateMutex);
    if (state != NodeState::LEADER) return;
    
    for (const auto& peer : peers) {
        if (peer.id == nodeId) continue;
        
        // 获取要发送给该节点的日志条目
        int64_t prevLogIndex = nextIndex[peer.id] - 1;
        int64_t prevLogTerm = 0;
        
        if (prevLogIndex > 0 && prevLogIndex <= log.size()) {
            prevLogTerm = log[prevLogIndex - 1].getTerm();
        }
        
        std::vector<LogEntry> entries;
        for (size_t i = nextIndex[peer.id] - 1; i < log.size(); i++) {
            entries.push_back(log[i]);
        }
        
        // 发送 AppendEntries RPC
        rpcClient.sendAppendEntries(peer.id, currentTerm, nodeId, prevLogIndex, prevLogTerm, entries, commitIndex,
            [this, peer](int peerId, int64_t term, bool success) {
                std::lock_guard<std::mutex> lock(stateMutex);
                
                // 如果已经不是领导者或者任期已经变化，忽略响应
                if (state != NodeState::LEADER || currentTerm != term) {
                    return;
                }
                
                // 如果收到的任期大于当前任期，变为 Follower
                if (term > currentTerm) {
                    becomeFollower(term);
                    return;
                }
                
                if (success) {
                    // 更新 nextIndex 和 matchIndex
                    nextIndex[peerId] = log.size() + 1;
                    matchIndex[peerId] = log.size();
                    
                    // 尝试提交新的日志条目
                    for (size_t n = commitIndex + 1; n <= log.size(); n++) {
                        if (log[n-1].getTerm() == currentTerm) {
                            // 统计该日志被复制到多少节点
                            int count = 1; // 包括自己
                            for (const auto& p : peers) {
                                if (p.id != nodeId && matchIndex[p.id] >= n) {
                                    count++;
                                }
                            }
                            
                            // 如果多数节点已复制，则提交
                            if (count > (peers.size() + 1) / 2) {
                                commitIndex = n;
                                std::cout << "Leader 提交日志索引 " << n << std::endl;
                                break;
                            }
                        }
                    }
                } else {
                    // 如果失败，减少 nextIndex 并重试
                    nextIndex[peerId] = std::max(1L, nextIndex[peerId] - 1);
                }
            }
        );
    }
}

bool RaftNode::handleRequestVote(int64_t term, int candidateId, int64_t lastLogIndex, int64_t lastLogTerm, bool& voteGranted) {
    std::lock_guard<std::mutex> lock(stateMutex);
    
    voteGranted = false;
    
    // 如果接收到的任期小于当前任期，拒绝投票
    if (term < currentTerm) {
        return true;
    }
    
    // 如果接收到的任期大于当前任期，转为 Follower
    if (term > currentTerm) {
        becomeFollower(term);
    }
    
    // 如果在当前任期还没有投票，或者已经投给了请求的候选人，并且候选人的日志至少和自己一样新，则投票
    if ((votedFor == -1 || votedFor == candidateId) && isLogUpToDate(lastLogIndex, lastLogTerm)) {
        votedFor = candidateId;
        storage.saveVote(votedFor);
        voteGranted = true;
        resetElectionTimer();
        std::cout << "节点 " << nodeId << " 投票给节点 " << candidateId << "，任期 " << currentTerm << std::endl;
    }
    
    return true;
}

bool RaftNode::isLogUpToDate(int64_t lastLogIndex, int64_t lastLogTerm) {
    int64_t myLastLogIndex = log.empty() ? 0 : log.back().getIndex();
    int64_t myLastLogTerm = log.empty() ? 0 : log.back().getTerm();
    
    // 如果任期不同，任期大的日志更新
    if (lastLogTerm != myLastLogTerm) {
        return lastLogTerm > myLastLogTerm;
    }
    
    // 如果任期相同，索引大的日志更新
    return lastLogIndex >= myLastLogIndex;
}

bool RaftNode::handleAppendEntries(int64_t term, int leaderId, int64_t prevLogIndex, int64_t prevLogTerm, 
                                  const std::vector<LogEntry>& entries, int64_t leaderCommit, bool& success) {
    std::lock_guard<std::mutex> lock(stateMutex);
    
    success = false;
    
    // 如果接收到的任期小于当前任期，拒绝附加日志
    if (term < currentTerm) {
        return true;
    }
    
    // 收到心跳，重置选举计时器
    resetElectionTimer();
    
    // 如果接收到的任期大于当前任期，转为 Follower
    if (term > currentTerm) {
        becomeFollower(term);
    } else if (state == NodeState::CANDIDATE) {
        // 如果是候选人，并且收到同任期的 AppendEntries，说明有其他节点已经成为领导者，转为 Follower
        becomeFollower(term);
    }
    
    // 检查 prevLogIndex 和 prevLogTerm 是否匹配
    if (prevLogIndex > 0) {
        if (log.size() < prevLogIndex || log[prevLogIndex-1].getTerm() != prevLogTerm) {
            // 日志不匹配，拒绝附加
            return true;
        }
    }
    
    // 附加新日志条目
    for (size_t i = 0; i < entries.size(); i++) {
        int64_t index = prevLogIndex + i + 1;
        
        // 如果已有的日志与新日志冲突，删除已有的日志及其之后的所有日志
        if (log.size() >= index) {
            if (log[index-1].getTerm() != entries[i].getTerm()) {
                log.resize(index - 1);
            } else {
                // 已有的日志与新日志匹配，跳过
                continue;
            }
        }
        
        // 附加新日志
        log.push_back(entries[i]);
    }
    
    // 持久化日志
    storage.saveLogs(log);
    
    // 更新 commitIndex
    if (leaderCommit > commitIndex) {
        commitIndex = std::min(leaderCommit, static_cast<int64_t>(log.size()));
        std::cout << "Follower 提交日志索引 " << commitIndex << std::endl;
    }
    
    success = true;
    return true;
}

void RaftNode::applyCommittedEntries() {
    std::lock_guard<std::mutex> lock(stateMutex);
    
    // 应用已提交但未应用的日志条目
    while (lastApplied < commitIndex && lastApplied < log.size()) {
        lastApplied++;
        const LogEntry& entry = log[lastApplied-1];
        
        // 应用到状态机
        if (stateMachine && entry.getType() != LogEntry::Type::NOOP) {
            stateMachine->apply(entry);
            std::cout << "应用日志条目 " << lastApplied << " 到状态机" << std::endl;
        }
    }
}

int64_t RaftNode::propose(const LogEntry& entry) {
    std::lock_guard<std::mutex> lock(stateMutex);
    
    if (state != NodeState::LEADER) {
        return -1;
    }

    LogEntry newEntry = entry;
    int64_t lastIndex = log.empty() ? 0 : log.back().getIndex();
    
    newEntry.setTerm(currentTerm);
    newEntry.setIndex(lastIndex + 1);
    
    log.push_back(newEntry);
    
    // 尝试复制到其他节点
    for (const auto& peer : peers) {
        if (peer.id != nodeId) {
            sendHeartbeats();  // 立即发送心跳/日志
        }
    }
    
    return newEntry.getIndex();
}

bool RaftNode::isLeader() const {
    return state == NodeState::LEADER;
}

std::string RaftNode::getLeaderAddress() const {
    // 在实际实现中，应该维护当前领导者的地址
    // 这里简化处理，返回空字符串
    return "";
}

int64_t RaftNode::getLastCommittedIndex() const {
    return commitIndex;
}

bool RaftNode::addServer(const NodeInfo& newNode) {
    std::lock_guard<std::mutex> lock(stateMutex);
    
    // 只有领导者可以添加新服务器
    if (state != NodeState::LEADER) {
        return false;
    }
    
    // 检查是否已存在
    for (const auto& peer : peers) {
        if (peer.id == newNode.id) {
            return false;
        }
    }
    
    // 添加新节点
    peers.push_back(newNode);
    nextIndex[newNode.id] = log.size() + 1;
    matchIndex[newNode.id] = 0;
    
    std::cout << "添加新节点 " << newNode.id << " 到集群" << std::endl;
    
    // 在实际实现中，应该创建一个特殊的配置变更日志条目
    // 这里简化处理，直接返回成功
    return true;
}