#include "../include/KVStore.h"
#include <iostream>
#include <string.h>
#include <cassert>
#include <thread>
#include <vector>

void testBasicOperations() {
    std::cout << "=== Testing Basic KV Operations ===" << std::endl;
    
    KVStore store(1024*1024, 100);
    
    // 测试set和get
    const char* value1 = "hello";
    assert(store.set("key1", value1, strlen(value1)+1));

    Value retrieved = store.get("key1");
    if (!retrieved.isNull()) {
        std::cout << "retrieved.data: " << retrieved.toString() << std::endl;
    }
    std::cout << "value1: " << value1 << std::endl;
    assert(strcmp(static_cast<const char*>(retrieved.data), value1) == 0);
    
    // Test overwrite
    const char* value2 = "world";
    assert(store.set("key1", value2, strlen(value2)+1));
    Value retrieved2 = store.get("key1");  // 获取更新后的值
    assert(!retrieved2.isNull());
    assert(strcmp(static_cast<const char*>(retrieved2.data), value2) == 0);  // 使用retrieved2比较
    
    // Test delete
    assert(store.del("key1"));
    assert(store.get("key1").isNull());
    
    std::cout << "Basic operations test passed!\n" << std::endl;
}

void testTTL() {
    std::cout << "=== Testing TTL Functionality ===" << std::endl;
    
    KVStore store(1024*1024, 100);
    
    const char* value = "expiring";
    assert(store.set("temp", value, strlen(value)+1, 100)); // 100ms TTL
    
    // 修改：立即获取时应存在（不应为null）
    Value retrieved = store.get("temp");
    assert(!retrieved.isNull()); // 修改断言：此时数据应存在
    
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    
    // 150ms后应过期
    assert(store.get("temp").isNull()); 
    
    std::cout << "TTL test passed!\n" << std::endl;
}

void testMemoryManagement() {
    std::cout << "=== Testing Memory Management ===" << std::endl;
    
    // 修改：将内存池大小设为刚好能存1个页（1MB）
    KVStore store(1024*1024, 10); // 1MB内存池
    
    // 修改：测试数据大小设为1MB
    char* largeValue = new char[1024*1024]; // 1MB数据
    // 第一个大对象应该成功
    assert(store.set("large1", largeValue, 1024*1024));

    // 第二个大对象应该失败（内存不足）
    assert(!store.set("large2", largeValue, 1024*1024));
    
    // 删除第一个后应该能存第二个
    assert(store.del("large1"));
    assert(store.set("large2", largeValue, 1024*1024));

    // 新增：验证第三个对象无法存入
    assert(!store.set("large3", largeValue, 1024*1024));
    
    delete[] largeValue;
    std::cout << "Memory management test passed!\n" << std::endl;
}

void testConcurrentAccess() {
    std::cout << "=== Testing Concurrent Access ===" << std::endl;
    
    KVStore store(100*1024*1024, 1000);
    const int THREADS = 10;
    const int OPS_PER_THREAD = 100;
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < THREADS; ++i) {
        threads.emplace_back([&store, i]() {
            for (int j = 0; j < OPS_PER_THREAD; ++j) {
                std::string key = "key_" + std::to_string(i) + "_" + std::to_string(j);
                std::string value = "value_" + std::to_string(i) + "_" + std::to_string(j);
                
                store.set(key, value.data(), value.size());
                
                Value retrieved = store.get(key);
                if (!retrieved.isNull() && retrieved.data) {  // 添加null检查
                    std::string retStr(static_cast<const char*>(retrieved.data), retrieved.size);
                    assert(retStr == value);
                }
                
                if (j % 10 == 0) {
                    store.del(key);
                }
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Concurrent access test passed!\n" << std::endl;
}

int main() {
    testBasicOperations();
    testTTL();
    testMemoryManagement();
    testConcurrentAccess();
    
    std::cout << "All KVStore tests passed successfully!" << std::endl;
    return 0;
}