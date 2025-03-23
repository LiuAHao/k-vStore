#include "PersistentStorage.h"
#include <iostream>

PersistentStorage::PersistentStorage(const std::string& path)
    : dbPath(path) {
    // 初始化 LevelDB
    leveldb::Options options;
    options.create_if_missing = true;
    
    leveldb::Status status = leveldb::DB::Open(options, dbPath, &db);
    if (!status.ok()) {
        std::cerr << "无法打开持久化存储: " << status.ToString() << std::endl;
        db = nullptr;
    }
}

PersistentStorage::~PersistentStorage() {
    if (db) {
        delete db;
    }
}

bool PersistentStorage::saveTerm(int64_t term) {
    if (!db) return false;
    
    leveldb::Status status = db->Put(leveldb::WriteOptions(), "currentTerm", std::to_string(term));
    return status.ok();
}

bool PersistentStorage::saveVote(int64_t candidateId) {
    if (!db) return false;
    
    leveldb::Status status = db->Put(leveldb::WriteOptions(), "votedFor", std::to_string(candidateId));
    return status.ok();
}

bool PersistentStorage::saveLogs(const std::vector<LogEntry>& logs) {
    if (!db) return false;
    
    // 保存日志条目数量
    leveldb::Status status = db->Put(leveldb::WriteOptions(), "logCount", std::to_string(logs.size()));
    if (!status.ok()) return false;
    
    // 保存每个日志条目
    for (size_t i = 0; i < logs.size(); i++) {
        status = db->Put(leveldb::WriteOptions(), "log_" + std::to_string(i), logs[i].serialize());
        if (!status.ok()) return false;
    }
    
    return true;
}

int64_t PersistentStorage::loadTerm() {
    if (!db) return 0;
    
    std::string value;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), "currentTerm", &value);
    
    if (status.ok()) {
        return std::stoll(value);
    } else {
        return 0; // 默认值
    }
}

int64_t PersistentStorage::loadVote() {
    if (!db) return -1;
    
    std::string value;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), "votedFor", &value);
    
    if (status.ok()) {
        return std::stoll(value);
    } else {
        return -1; // 表示未投票
    }
}

std::vector<LogEntry> PersistentStorage::loadLogs() {
    std::vector<LogEntry> logs;
    if (!db) return logs;
    
    // 加载日志条目数量
    std::string countStr;
    leveldb::Status status = db->Get(leveldb::ReadOptions(), "logCount", &countStr);
    
    if (!status.ok()) return logs;
    
    size_t count = std::stoull(countStr);
    
    // 加载每个日志条目
    for (size_t i = 0; i < count; i++) {
        std::string logData;
        status = db->Get(leveldb::ReadOptions(), "log_" + std::to_string(i), &logData);
        
        if (status.ok()) {
            logs.push_back(LogEntry::deserialize(logData));
        } else {
            std::cerr << "加载日志条目 " << i << " 失败" << std::endl;
            break;
        }
    }
    
    return logs;
}
