#include "KvStateMachine.h"
#include <iostream>

KvStateMachine::KvStateMachine() {
    // 初始化LevelDB
    leveldb::Options options;
    options.create_if_missing = true;
    
    leveldb::Status status = leveldb::DB::Open(options, "/tmp/kvstore_db", &database);
    if (!status.ok()) {
        std::cerr << "无法打开数据库: " << status.ToString() << std::endl;
        database = nullptr;
    }
}

KvStateMachine::~KvStateMachine() {
    if (database) {
        delete database;
    }
}

void KvStateMachine::apply(const LogEntry& entry) {
    if (!database) {
        std::cerr << "数据库未初始化" << std::endl;
        return;
    }
    
    leveldb::Status status;
    
    switch (entry.getType()) {
        case LogEntry::Type::SET:
            status = database->Put(leveldb::WriteOptions(), entry.getKey(), entry.getValue());
            if (!status.ok()) {
                std::cerr << "SET操作失败: " << status.ToString() << std::endl;
            }
            break;
            
        case LogEntry::Type::DELETE:
            status = database->Delete(leveldb::WriteOptions(), entry.getKey());
            if (!status.ok()) {
                std::cerr << "DELETE操作失败: " << status.ToString() << std::endl;
            }
            break;
            
        case LogEntry::Type::GET:
            // GET操作通常不需要应用到状态机，因为它不改变状态
            // 这里只是记录一下
            std::cout << "收到GET操作日志: " << entry.getKey() << std::endl;
            break;
            
        case LogEntry::Type::NOOP:
            // 空操作，不做任何事
            break;
    }
}

std::string KvStateMachine::get(const std::string& key) {
    if (!database) {
        std::cerr << "数据库未初始化" << std::endl;
        return "";
    }
    
    std::string value;
    leveldb::Status status = database->Get(leveldb::ReadOptions(), key, &value);
    
    if (status.ok()) {
        return value;
    } else if (status.IsNotFound()) {
        return ""; // 键不存在
    } else {
        std::cerr << "GET操作失败: " << status.ToString() << std::endl;
        return "";
    }
}

