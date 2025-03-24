#include "PersistentStorage.h"
#include <iostream>
#include <leveldb/write_batch.h>
#include <leveldb/env.h>
#include <leveldb/cache.h>
#include <leveldb/filter_policy.h>  // 添加filter_policy头文件

PersistentStorage::PersistentStorage(const std::string& path) 
    : dbPath(path), db(nullptr) {
    leveldb::Options options;
    options.create_if_missing = true;
    options.filter_policy = leveldb::NewBloomFilterPolicy(10);  // 修正BloomFilter调用
    options.block_cache = leveldb::NewLRUCache(64 * 1024 * 1024); // 64MB缓存
    options.filter_policy = leveldb::NewBloomFilterPolicy(10);
    
    leveldb::Status status = leveldb::DB::Open(options, dbPath, &db);
    if (!status.ok()) {
        std::cerr << "持久化存储初始化失败: " << status.ToString() << std::endl;
        db = nullptr;
    }
}

PersistentStorage::~PersistentStorage() {
    delete db;
    delete leveldb::Options().block_cache;
}

bool PersistentStorage::saveTerm(int64_t term) {
    std::lock_guard<std::mutex> lock(dbMutex);
    return db->Put(leveldb::WriteOptions(), "currentTerm", std::to_string(term)).ok();
}

bool PersistentStorage::saveVote(int64_t candidateId) {
    std::lock_guard<std::mutex> lock(dbMutex);
    return db->Put(leveldb::WriteOptions(), "votedFor", std::to_string(candidateId)).ok();
}

bool PersistentStorage::saveLogs(const std::vector<LogEntry>& logs) {
    std::lock_guard<std::mutex> lock(dbMutex);
    leveldb::WriteBatch batch;
    
    // 使用日志索引作为key
    for (const auto& entry : logs) {
        batch.Put("log_" + std::to_string(entry.getIndex()), 
                 entry.serialize());
    }
    
    // 保存最大日志索引
    int64_t maxIndex = logs.empty() ? 0 : logs.back().getIndex();
    batch.Put("maxLogIndex", std::to_string(maxIndex));
    
    return db->Write(leveldb::WriteOptions(), &batch).ok();
}

bool PersistentStorage::saveSnapshot(int64_t lastIncludedIndex, 
                                   int64_t lastIncludedTerm,
                                   const std::string& snapshotPath) {
    std::lock_guard<std::mutex> lock(dbMutex);
    leveldb::WriteBatch batch;
    
    batch.Put("snapshot_lastIndex", std::to_string(lastIncludedIndex));
    batch.Put("snapshot_lastTerm", std::to_string(lastIncludedTerm));
    batch.Put("snapshot_file", snapshotPath);
    
    // 删除已快照的旧日志
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->Seek("log_"); it->Valid() && it->key().starts_with("log_"); it->Next()) {
        int64_t logIndex = std::stoll(it->key().ToString().substr(4));
        if (logIndex <= lastIncludedIndex) {
            batch.Delete(it->key());
        }
    }
    delete it;
    
    return db->Write(leveldb::WriteOptions(), &batch).ok();
}

// 加载方法保持不变，增加日志扫描逻辑
std::vector<LogEntry> PersistentStorage::loadLogs() {
    std::vector<LogEntry> logs;
    std::lock_guard<std::mutex> lock(dbMutex);
    
    std::string maxIndexStr;
    db->Get(leveldb::ReadOptions(), "maxLogIndex", &maxIndexStr);
    
    int64_t maxIndex = maxIndexStr.empty() ? 0 : std::stoll(maxIndexStr);
    
    for (int64_t i = 1; i <= maxIndex; ++i) {
        std::string logData;
        if (db->Get(leveldb::ReadOptions(), "log_" + std::to_string(i), &logData).ok()) {
            logs.push_back(LogEntry::deserialize(logData));
        }
    }
    return logs;
}

// 新增批量操作接口
void PersistentStorage::batchPut(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(dbMutex);
    batch.Put(key, value);
}

bool PersistentStorage::commitBatch() {
    std::lock_guard<std::mutex> lock(dbMutex);
    leveldb::Status status = db->Write(leveldb::WriteOptions(), &batch);
    batch.Clear();
    return status.ok();
}
