#include "StateMachine.h"
#include "leveldb/db.h"
#include <string>
#include <iostream>
#include <memory>

class KvStateMachine : public StateMachine {
private:
    std::unique_ptr<leveldb::DB> database;  // 使用智能指针管理数据库实例
    leveldb::DB* database;      // 数据库实例
public:
    KvStateMachine();
    ~KvStateMachine();
    void apply(const LogEntry& entry) override;
};
