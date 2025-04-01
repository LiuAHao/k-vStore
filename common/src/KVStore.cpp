#include"../include/KVStore.h"

KVStore::KVStore(size_t max_memory, size_t lru_capacity){
    //利用make_unique初始化智能指针
    allocator_ = std::make_unique<SlabAllocator>(max_memory);
    ttl_manager_ = std::make_unique<TTLManager>();
    lru_cache_ = std::make_unique<LRUCache>(lru_capacity);
}

KVStore::~KVStore(){
}

bool KVStore::set(const std::string& key, const void* value, size_t size, long long ttl_ms){
    std::lock_guard<std::recursive_mutex> lock(storage_mutex_);

    void* ptr = allocator_->allocate(size);
    if(!ptr) return false;

    //拷贝数据到分配的内存
    memcpy(ptr, value, size);

    Value val;
    val.data = ptr;
    val.size = size;

    //处理已经有的键
    auto it = storage_.find(key);
    if(it != storage_.end()){
        //释放旧的内存
        allocator_->deallocate(it->second.data);
        //从LRU缓存中删去
        lru_cache_->erase(key);
        //从TTL中去除
        ttl_manager_->removeTTL(key);

        it->second = val;
    }
    
    storage_[key] = val;
    if(ttl_ms > 0){
        ttl_manager_->setTTL(key,TTLManager::msToimePoint(ttl_ms));
    }
    lru_cache_->put(key, val);
    return true;
}

bool KVStore::get(const std::string& key, Value& value){
    std::lock_guard<std::recursive_mutex> lock(storage_mutex_);
    if(ttl_manager_->isExpired(key)){
        del(key);
        return false;
    }
    std::cout<<"到了这里1"<<std::endl;
    //找到的值会存在value
    auto is_find = lru_cache_->get(key, value); //出了问题
    if(is_find == true) return true;

    std::cout<<"到了这里1"<<std::endl;
    auto it = storage_.find(key);
    if(it == storage_.end()) return false;
    value = it->second;
    lru_cache_->put(key,value);
    return true;
}

bool KVStore::del(const std::string& key){
    std::lock_guard<std::recursive_mutex> lock(storage_mutex_);
    lru_cache_->erase(key);

    auto it = storage_.find(key);
    if(it == storage_.end()) return false;

    allocator_->deallocate(it->second.data);
    storage_.erase(it->first);
    return true;

}
    
void KVStore::cleanup(){
    std::lock_guard<std::recursive_mutex> lock(storage_mutex_);

    std::vector<std::string> to_clean = ttl_manager_->cleanupExpired();
    for(auto& k:to_clean){
        lru_cache_->erase(k);
        auto it = storage_.find(k);
        if(it == storage_.end())continue;

        allocator_->deallocate(it->second.data);
        storage_.erase(it->first);
    }
}

size_t KVStore::getUseMemory() const{
    return allocator_->getUsedMemory();
}

size_t KVStore::getFreeMemory() const{
    return allocator_->getFreeMemory();
}

