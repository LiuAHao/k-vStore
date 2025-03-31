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
    if(capacity_ == 0) return;
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = lookup_.find(key);
    if(it != lookup_.end()){
        // 键已存在，更新值并移动到前端
        items_.splice(items_.begin(), items_, it->second);
        it->second->second = value;
        return;
    }

    // 键不存在，需要插入新项
    if(items_.size() >= capacity_){
        // 移除最久未使用的项,不调用原函数的erase()会导致死锁
        lookup_.erase(items_.back().first);
        items_.pop_back();
    }

    items_.emplace_front(key,value);
    lookup_[key] = items_.begin();
}
    
template<typename K, typename V>
void LRUCache<K, V>::erase(const K& key){
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = lookup_.find(key);
    if(it != lookup_.end()){
        items_.erase(lookup_[key]);
        lookup_.erase(key);
    }
    
}
    
template<typename K, typename V>
size_t LRUCache<K, V>::size() const{
    std::lock_guard<std::mutex> lock(mutex_);
    return items_.size();
}


// #include <iostream>
// #include <thread>
// #include <vector>
// #include <cassert>
// #include "../include/LRUCache.h"

// void testBasicOperations() {
//     std::cout << "=== 开始基础操作测试 ===" << std::endl;
    
//     LRUCache<std::string, int> cache(3);
    
//     // 测试插入和获取
//     cache.put("one", 1);
//     cache.put("two", 2);
//     cache.put("three", 3);
    
//     int value = 0;
//     assert(cache.get("one", value) && value == 1);
//     assert(cache.get("two", value) && value == 2);
//     assert(cache.get("three", value) && value == 3);
//     std::cout << "测试1通过: 基础插入和查询" << std::endl;
    
//     // 测试LRU淘汰
//     cache.put("four", 4); // 应该淘汰"one"
//     assert(!cache.get("one", value));
//     assert(cache.get("two", value) && value == 2);
//     std::cout << "测试2通过: LRU淘汰机制" << std::endl;
    
//     // 测试更新现有键
//     cache.put("two", 22);
//     assert(cache.get("two", value) && value == 22);
//     std::cout << "测试3通过: 更新现有键" << std::endl;
    
//     // 测试删除
//     cache.erase("three");
//     assert(!cache.get("three", value));
//     assert(cache.size() == 2);
//     std::cout << "测试4通过: 删除键值" << std::endl;
    
//     std::cout << "=== 基础操作测试通过 ===" << std::endl << std::endl;
// }

// void testBoundaryConditions() {
//     std::cout << "=== 开始边界条件测试 ===" << std::endl;
    
//     // 测试容量为0
//     LRUCache<std::string, int> zeroCache(0);
//     zeroCache.put("test", 1);
//     int value = 0;
//     assert(!zeroCache.get("test", value));
//     std::cout << "测试1通过: 容量为0的缓存" << std::endl;
    
//     // 测试容量为1
//     LRUCache<int, int> singleCache(1);
//     singleCache.put(1, 100);
//     assert(singleCache.get(1, value) && value == 100);
//     singleCache.put(2, 200); // 应该淘汰1
//     assert(!singleCache.get(1, value));
//     assert(singleCache.get(2, value) && value == 200);
//     std::cout << "测试2通过: 容量为1的缓存" << std::endl;
    
//     std::cout << "=== 边界条件测试通过 ===" << std::endl << std::endl;
// }

// void testConcurrentAccess() {
//     std::cout << "=== 开始并发访问测试 ===" << std::endl;
    
//     LRUCache<int, int> cache(100);
//     const int THREAD_COUNT = 10;
//     const int OPERATIONS_PER_THREAD = 1000;
//     std::vector<std::thread> threads;
    
//     // 启动多个线程进行并发操作
//     for (int i = 0; i < THREAD_COUNT; ++i) {
//         threads.emplace_back([&cache, i]() {
//             for (int j = 0; j < OPERATIONS_PER_THREAD; ++j) {
//                 int key = i * OPERATIONS_PER_THREAD + j;
//                 cache.put(key, key * 10);
                
//                 int value = 0;
//                 if (cache.get(key, value)) {
//                     assert(value == key * 10);
//                 }
                
//                 if (j % 10 == 0) {
//                     cache.erase(key);
//                 }
//             }
//         });
//     }
//     // 等待所有线程完成
//     for (auto& t : threads) {
//         t.join();
//     }
    
//     // 验证最终状态
//     assert(cache.size() <= 100);
//     std::cout << "测试通过: 并发操作无数据竞争" << std::endl;
    
//     std::cout << "=== 并发访问测试通过 ===" << std::endl << std::endl;
// }

// void testLRUOrder() {
//     std::cout << "=== 开始LRU顺序测试 ===" << std::endl;
    
//     LRUCache<int, std::string> cache(3);
    
//     cache.put(1, "one");
//     cache.put(2, "two");
//     cache.put(3, "three");
    
//     // 访问1，使其成为最近使用的
//     std::string value;
//     cache.get(1, value);
    
//     // 插入4，应该淘汰2（因为2是最近最少使用的）
//     cache.put(4, "four");
    
//     assert(cache.get(1, value));  // 1应该存在
//     assert(!cache.get(2, value)); // 2应该被淘汰
//     assert(cache.get(3, value));  // 3应该存在
//     assert(cache.get(4, value));  // 4应该存在
//     std::cout << "测试通过: LRU顺序正确" << std::endl;
    
//     std::cout << "=== LRU顺序测试通过 ===" << std::endl << std::endl;
// }

// int main() {
//     try {
//         testBasicOperations();
//         testBoundaryConditions();
//         testLRUOrder();
//         testConcurrentAccess();
        
//         std::cout << "所有测试通过！LRUCache 实现正确。" << std::endl;
//         return 0;
//     } catch (const std::exception& e) {
//         std::cerr << "测试失败: " << e.what() << std::endl;
//         return 1;
//     }
// }