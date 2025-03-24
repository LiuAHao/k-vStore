#pragma once
#include <string>
#include <vector>
#include <mutex>
#include "leveldb/db.h"
#include "leveldb/write_batch.h"  // 添加write_batch头文件
#include "LogEntry.h"

class PersistentStorage {
private:
    std::mutex dbMutex;          // 线程安全锁
    std::string dbPath;
    leveldb::DB* db;
    leveldb::WriteBatch batch;   // 批量操作支持

    // 快照元数据
    struct SnapshotMetadata {
        int64_t lastIncludedIndex;
        int64_t lastIncludedTerm;
        std::string snapshotFile;
    };

public:
    explicit PersistentStorage(const std::string& path);
    ~PersistentStorage();
    
    // 核心方法
    bool saveTerm(int64_t term);
    bool saveVote(int64_t candidateId);
    bool saveLogs(const std::vector<LogEntry>& logs);
    bool saveSnapshot(int64_t lastIncludedIndex, int64_t lastIncludedTerm, 
                     const std::string& snapshotPath);

    int64_t loadTerm();
    int64_t loadVote();
    std::vector<LogEntry> loadLogs();
    bool loadSnapshot(std::string& outSnapshotPath);

    // 批量操作接口
    void batchPut(const std::string& key, const std::string& value);
    bool commitBatch();
};