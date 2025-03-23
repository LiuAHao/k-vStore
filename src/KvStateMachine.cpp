#include "KvStateMachine.h"

KvStateMachine::KvStateMachine() 
{
    // 初始化数据库
    database = new leveldb::DB();
}

KvStateMachine::~KvStateMachine() {
}

void KvStateMachine::apply(const LogEntry& entry) {
    // 执行具体操作：SET/GET/DELETE
    switch (entry.type) {
        case LogEntry::Type::SET:
            database->Put(entry.key, entry.value);
            break;
        case LogEntry::Type::GET:
            database->Get(entry.key);
            break;
    }
}

