#pragma once

#include<string>
#include<memory>
#include "SlabAllocator.h"
#include "TTLManager.h"
#include "LRUCache.h"

class KVStore{
public:
    struct Value{
        void* data;
        size_t size;
    };

    //初始化KV存储，指定最大内存和LRU容量
    KVStore(size_t max_memory, size_t lru_capacity);
    ~KVStore();

    // 存储键值对，可选TTL(毫秒)
    bool set(const std::string& key, const void* value, size_t size, long long ttl_ms = 0);
    
    // 获取键值对
    bool get(const std::string& key, Value& value);
    
    // 删除键值对
    bool del(const std::string& key);
    
    // 清理过期键
    void cleanup();
    
    // 获取内存使用统计
    size_t getUseMemory() const;
    size_t getFreeMemory() const;

private:
    std::unique_ptr<SlabAllocator> allocator_;
    std::unique_ptr<TTLManager> ttl_manager_;
    std::unique_ptr<LRUCache<std::string, Value>> lru_cache_;
    // 实际存储(简化版，实际可能需要更复杂的结构)
    std::unordered_map<std::string, Value> storage_;
    mutable std::mutex storage_mutex_;
    // 从存储中移除键值对(不通知LRU)
    void removeFromStorage(const std::string& key);

};