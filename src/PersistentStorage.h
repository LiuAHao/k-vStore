#pragma once
#include <string>
#include <vector>
#include "leveldb/db.h"
#include "LogEntry.h"

class PersistentStorage {
private:
    std::string dbPath;
    leveldb::DB* db;  // LevelDB 数据库实例

public:
    PersistentStorage(const std::string& path);
    ~PersistentStorage();
    
    bool saveTerm(int64_t term);         // 保存当前任期
    bool saveVote(int64_t candidateId);  // 保存投票记录
    bool saveLogs(const std::vector<LogEntry>& logs); // 保存日志
    
    int64_t loadTerm();                  // 加载当前任期
    int64_t loadVote();                  // 加载投票记录
    std::vector<LogEntry> loadLogs();    // 加载日志
};