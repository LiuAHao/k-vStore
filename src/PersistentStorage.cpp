#include "PersistentStorage.h"

PersistentStorage::PersistentStorage(const std::string& path) 
{
    // 初始化持久化存储
    db = new leveldb::DB();
}
