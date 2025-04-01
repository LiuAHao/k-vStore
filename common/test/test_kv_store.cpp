#include "../include/KVStore.h"
#include <iostream>
#include <string.h>
#include <cassert>
#include <thread>
#include <vector>

void testBasicOperations() {
    std::cout << "=== Testing Basic KV Operations ===" << std::endl;
    
    KVStore store(1024*1024, 100); // 1MB内存，LRU容量100
    // Test set and get
    const char* value1 = "hello";
    assert(store.set("key1", value1, strlen(value1)+1));

    Value retrieved;
    assert(store.get("key1", retrieved));   //出了问题
    assert(strcmp(static_cast<const char*>(retrieved.data), value1) == 0);
    
    // Test overwrite
    const char* value2 = "world";
    assert(store.set("key1", value2, strlen(value2)+1));
    assert(store.get("key1", retrieved));
    assert(strcmp(static_cast<const char*>(retrieved.data), value2) == 0);
    
    // Test delete
    assert(store.del("key1"));
    assert(!store.get("key1", retrieved));
    
    std::cout << "Basic operations test passed!\n" << std::endl;
}

void testTTL() {
    std::cout << "=== Testing TTL Functionality ===" << std::endl;
    
    KVStore store(1024*1024, 100);
    
    const char* value = "expiring";
    assert(store.set("temp", value, strlen(value)+1, 100)); // 100ms TTL
    
    Value retrieved;
    assert(store.get("temp", retrieved)); // Should still exist
    
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    
    assert(!store.get("temp", retrieved)); // Should have expired
    
    std::cout << "TTL test passed!\n" << std::endl;
}

void testMemoryManagement() {
    std::cout << "=== Testing Memory Management ===" << std::endl;
    
    KVStore store(1024, 10); // Very small memory pool (1KB)
    
    char largeValue[512];
    assert(store.set("large1", largeValue, sizeof(largeValue)));
    
    // Second large value should fail (not enough memory)
    assert(!store.set("large2", largeValue, sizeof(largeValue)));
    
    // After deleting first, should succeed
    assert(store.del("large1"));
    assert(store.set("large2", largeValue, sizeof(largeValue)));
    
    std::cout << "Memory management test passed!\n" << std::endl;
}

void testConcurrentAccess() {
    std::cout << "=== Testing Concurrent Access ===" << std::endl;
    
    KVStore store(1024*1024, 1000);
    const int THREADS = 10;
    const int OPS_PER_THREAD = 100;
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < THREADS; ++i) {
        threads.emplace_back([&store, i]() {
            for (int j = 0; j < OPS_PER_THREAD; ++j) {
                std::string key = "key_" + std::to_string(i) + "_" + std::to_string(j);
                std::string value = "value_" + std::to_string(i) + "_" + std::to_string(j);
                
                store.set(key, value.data(), value.size());
                
                Value retrieved;
                if (store.get(key, retrieved)) {
                    std::string retStr(static_cast<const char*>(retrieved.data));
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