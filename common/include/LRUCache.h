#pragma once
#include <list>
#include <unordered_map>
#include <string>
#include <mutex>

template<typename K, typename V>
class LRUCache {
public:
    explicit LRUCache(size_t capacity):capacity_(capacity){}
    // 获取键值，如果存在则提升到最近使用
    bool get(const K& key, V& value);
    
    // 添加键值，如果已存在则更新
    void put(const K& key, const V& value);
    
    // 删除键值
    void erase(const K& key);
    
    // 获取缓存大小
    size_t size() const;
private:
    //List容器，底层实现为双向链表
    using ListType = std::list<std::pair<K,V>>; 
    using ListIter = typename ListType::iterator;

    size_t capacity_;
    ListType items_; 
    //通过键，快速查找对应的指针
    std::unordered_map<K, ListIter> lookup_;   

    mutable std::mutex mutex_;

};