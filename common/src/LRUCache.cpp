#include"../include/LRUCache.h"

template<typename K, typename V>
bool LRUCache<K, V>::get(const K& key, V& value){
    //加锁防止冲突
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = lookup_.find(key);
    if(it == lookup_.end())return false;

    //将查到的节点放到列表前端，表示最近使用;
    //splice表示剪切一部分到另一部分，下面为直接将it的值剪到头部
    items_.splice(items_.begin(), items_, it->second);
    //更新value的值
    value = it->second->second;
    return true;
}

template<typename K, typename V>
void LRUCache<K, V>::put(const K& key, const V& value){
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = lookup_.find(key);
    if(it != lookup_.end()){
        // 键已存在，更新值并移动到前端
        items_.splice(items_.begin(), items_, it->second);
        it->second->second = value;
        return;
    }

    // 键不存在，需要插入新项
    if(size() >= capacity_){
        // 移除最久未使用的项
        erase(items_.back().first);
    }

    items_.emplace_front(key,value);
    lookup_[key] = items_.begin();
}
    
template<typename K, typename V>
void LRUCache<K, V>::erase(const K& key){
    auto it = lookup_.find(key);
    if(it != lookup_.end()){
        items_.erase(lookup_[key]);
        lookup_.erase(key);
    }
    
}
    
template<typename K, typename V>
size_t LRUCache<K, V>::size() const{
    return items_.size()
}