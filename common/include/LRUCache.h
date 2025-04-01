#pragma once
#include <list>
#include <unordered_map>
#include <string>
#include <mutex>
#include "Value.h"
#include "KVStore.h"

class KVStore;

class LRUCache {
public:
    LRUCache(size_t capacity);
    // 获取键值，如果存在则提升到最近使用
    bool get(const std::string& key, Value& value);
    
    // 添加键值，如果已存在则更新
    void put(const std::string& key, const Value& value);
    
    // 删除键值
    void erase(const std::string& key);
    
    // 获取缓存大小
    size_t size() const;
private:
    using Value = ::Value;
    //List容器，底层实现为双向链表
    using ListType = std::list<std::pair<std::string, Value>>; 
    using ListIter = typename ListType::iterator;

    size_t capacity_;
    ListType items_; 
    //通过键，快速查找对应的指针
    std::unordered_map<std::string, ListIter> lookup_;   

    mutable std::mutex mutex_;

};

