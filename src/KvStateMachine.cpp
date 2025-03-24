#include "KvStateMachine.h"
#include <iostream>
#include <fstream>
#include <leveldb/write_batch.h>

void KvStateMachine::apply(const LogEntry& entry) {
    leveldb::WriteBatch batch;
    switch(entry.getType()) {
        case LogEntry::Type::SET:
            batch.Put(entry.getKey(), entry.getValue());
            break;
        case LogEntry::Type::DELETE:
            batch.Delete(entry.getKey());
            break;
        case LogEntry::Type::NOOP:
            // 空操作，不做任何事
            break;
    }
    
    leveldb::Status status = db->Write(leveldb::WriteOptions(), &batch);
    if (!status.ok()) {
        std::cerr << "状态机应用失败: " << status.ToString() << std::endl;
    }
}

bool KvStateMachine::createSnapshot(const std::string& snapshotPath) {
    // 简化实现：直接将当前状态写入文件
    std::ofstream snapshot(snapshotPath, std::ios::binary);
    if (!snapshot) return false;

    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        snapshot << it->key().ToString() << ":" << it->value().ToString() << "\n";
    }
    delete it;
    return true;
}

