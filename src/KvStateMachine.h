#pragma once
#include <string>
#include "LogEntry.h"
#include <leveldb/db.h>

class KvStateMachine {
public:
    KvStateMachine(const std::string& dbPath);
    ~KvStateMachine();

    // 移除override关键字
    void apply(const LogEntry& entry);
    bool createSnapshot(const std::string& snapshotPath);
    bool applySnapshot(const std::string& snapshotPath);

private:
    leveldb::DB* db;
    void initializeDB(const std::string& dbPath);
};
