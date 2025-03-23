#pragma once
#include "StateMachine.h"
#include "LogEntry.h"
#include "leveldb/db.h"
#include <string>
#include <iostream>

class KvStateMachine : public StateMachine
{
private:
    leveldb::DB* database;      // 数据库实例
public:
    KvStateMachine();
    ~KvStateMachine();
    void apply(const LogEntry& entry) override;
    std::string get(const std::string& key);
};
