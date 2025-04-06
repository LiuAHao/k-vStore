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

    // 使用 SlabAllocator 分配内存
    void* ptr = allocator_->allocate(size);
    if(!ptr) return false;
    
    // 拷贝数据到分配的内存
    memcpy(ptr, value, size);

    Value val;
    val.data = ptr;
    val.size = size;

    // 处理已存在的键
    auto it = storage_.find(key);
    if(it != storage_.end()){
        allocator_->deallocate(it->second.data);
        lru_cache_->erase(key);
        ttl_manager_->removeTTL(key);
        it->second = val;
    }

    storage_[key] = val;
    lru_cache_->put(key, val);

    if(ttl_ms > 0){
        ttl_manager_->setTTL(key, TTLManager::msToimePoint(ttl_ms));
    }

    return true;
}

Value KVStore::get(const std::string& key){
    std::lock_guard<std::recursive_mutex> lock(storage_mutex_);
    if(ttl_manager_->isExpired(key)){
        del(key);
        return Value();
    }

    // 先尝试从LRU缓存获取
    Value cached_value = lru_cache_->get(key);
    
    if(!cached_value.isNull()){
        // 验证内存是否有效
        if (cached_value.data && cached_value.size > 0) {
            // 分配新内存并复制数据
            void* new_ptr = allocator_->allocate(cached_value.size);
            if (new_ptr) {
                memcpy(new_ptr, cached_value.data, cached_value.size);
                Value new_value;
                new_value.data = new_ptr;
                new_value.size = cached_value.size;
                
                // 安全打印字符串内容
                if (new_value.size > 0) {
                    std::string str(static_cast<const char*>(new_value.data), new_value.size);
                }
                return new_value;
            }
        }
    }

    // 如果缓存未命中，从存储中获取
    auto it = storage_.find(key);
    if(it == storage_.end()) return Value();
    
    // 从存储中获取时也需要复制数据
    void* new_ptr = allocator_->allocate(it->second.size);
    if (!new_ptr) return Value();
    
    memcpy(new_ptr, it->second.data, it->second.size);
    Value new_value;
    new_value.data = new_ptr;
    new_value.size = it->second.size;
    
    // 更新缓存
    lru_cache_->put(key, it->second);
    return new_value;
}

bool KVStore::del(const std::string& key) {
    std::lock_guard<std::recursive_mutex> lock(storage_mutex_);
    lru_cache_->erase(key);
    ttl_manager_->removeTTL(key);

    auto it = storage_.find(key);
    if(it == storage_.end()) return false;

    // 确保Value对象被正确释放
    allocator_->deallocate(it->second.data);
    storage_.erase(it);
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

Value KVStore::put(const std::string& key, const Value& value) {
    std::lock_guard<std::recursive_mutex> lock(storage_mutex_);

    // 分配新内存并复制数据
    void* new_ptr = allocator_->allocate(value.size);
    if(!new_ptr) return Value();  // 分配失败返回空Value

    memcpy(new_ptr, value.data, value.size);

    Value new_value;
    new_value.data = new_ptr;
    new_value.size = value.size;

    // 处理已存在的键
    auto it = storage_.find(key);
    if(it != storage_.end()){
        Value old_value = it->second;
        it->second = new_value;
        lru_cache_->put(key, new_value);
        return old_value;
    }

    // 插入新键值对
    storage_[key] = new_value;
    lru_cache_->put(key, new_value);
    return Value();  // 新插入返回空Value
}

