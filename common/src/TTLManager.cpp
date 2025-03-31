#include"../include/TTLManager.h"

void TTLManager::setTTL(const std::string& key, TimePoint expire_time){
    std::lock_guard<std::mutex> lock(mutex_);
    TTL_map_[key] = expire_time;
}

long long TTLManager::get_TTL(const std::string& key)const{
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = TTL_map_.find(key);
    if(it == TTL_map_.end())return 0;   //没找着

    auto now = std::chrono::system_clock::now();
    if(now >= it->second)return 0;   //已过期
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
         it->second - now
    );
    
    return duration.count();
} 

bool TTLManager::isExpired(const std::string& key)const{
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = TTL_map_.find(key);
    if(it == TTL_map_.end())return true;

    return std::chrono::system_clock::now() >= it->second;
}

void TTLManager::removeTTL(const std::string& key){
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = TTL_map_.find(key);
    if(it == TTL_map_.end())return;

    TTL_map_.erase(it);
}

void TTLManager::cleanupExpired(){
    std::lock_guard<std::mutex> lock(mutex_);
    auto now = std::chrono::system_clock::now();
    std::vector<std::string> to_clean;
    for(auto& t:TTL_map_){
        if(t.second <= std::chrono::system_clock::now()){
            to_clean.push_back(t.first);
        }
    }
    for(int i = 0; i < to_clean.size(); i++){
        TTL_map_.erase(to_clean[i]);
    }
}
    

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cassert>


void runTests() {
    TTLManager ttl;
    auto now = std::chrono::system_clock::now();
    
    // ===== 测试1：基本TTL设置和查询 =====
    {
        auto expire_1s = now + std::chrono::seconds(1);
        ttl.setTTL("key1", expire_1s);
        
        // 刚设置不应过期
        assert(ttl.isExpired("key1") == false);
        assert(ttl.get_TTL("key1") > 0);
        
        // 不存在的key应返回已过期
        assert(ttl.isExpired("nonexistent") == true);
        assert(ttl.get_TTL("nonexistent") == 0);
        
        std::cout << "测试1通过：基本TTL设置和查询\n";
    }
    
    // ===== 测试2：精确时间计算 ===== 
    {
        auto start = std::chrono::system_clock::now();
        auto expire_500ms = start + std::chrono::milliseconds(500);
        ttl.setTTL("key2", expire_500ms);
        
        // 立即检查剩余时间应该在490-500ms范围内
        long long remaining = ttl.get_TTL("key2");
        assert(remaining > 490 && remaining <= 500);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // 100ms后检查剩余时间应该在390-400ms范围内
        remaining = ttl.get_TTL("key2");
        assert(remaining > 380 && remaining <= 400);
        
        std::cout << "测试2通过：精确时间计算\n";
    }
    
    // ===== 测试3：过期检查 =====
    {
        auto expire_past = now - std::chrono::seconds(1); // 设置过去时间
        ttl.setTTL("key3", expire_past);
        
        assert(ttl.isExpired("key3") == true);
        assert(ttl.get_TTL("key3") == 0);
        
        std::cout << "测试3通过：过期检查\n";
    }
    
    // ===== 测试4：TTL移除 =====
    {
        auto expire_5s = now + std::chrono::seconds(5);
        ttl.setTTL("key4", expire_5s);
        
        // 移除前应存在
        assert(ttl.isExpired("key4") == false);
        
        ttl.removeTTL("key4");
        
        // 移除后应视为已过期
        assert(ttl.isExpired("key4") == true);
        assert(ttl.get_TTL("key4") == 0);
        
        // 移除不存在的key不应报错
        ttl.removeTTL("nonexistent_key");
        
        std::cout << "测试4通过：TTL移除\n";
    }
    
    // ===== 测试5：清理过期键 =====
    {
        // 设置3个key：1个未过期，2个已过期
        ttl.setTTL("key5_valid", now + std::chrono::hours(1));
        ttl.setTTL("key5_expired1", now - std::chrono::seconds(1));
        ttl.setTTL("key5_expired2", now - std::chrono::seconds(2));
        
        // 清理前应能查到所有key
        assert(ttl.isExpired("key5_valid") == false);
        assert(ttl.isExpired("key5_expired1") == true);
        assert(ttl.isExpired("key5_expired2") == true);
        
        ttl.cleanupExpired();
        
        // 清理后过期key应被移除
        assert(ttl.isExpired("key5_valid") == false);  // 未过期的仍在
        assert(ttl.isExpired("key5_expired1") == true); // 已过期的查询返回true
        assert(ttl.isExpired("key5_expired2") == true); // 但实际已被删除
        
        std::cout << "测试5通过：清理过期键\n";
    }
    
    // ===== 测试6：线程安全 =====
    {
        const int THREAD_COUNT = 10;
        const int OPERATIONS_PER_THREAD = 1000;
        std::vector<std::thread> threads;
        
        auto worker = [&ttl](int threadId) {
            for (int i = 0; i < OPERATIONS_PER_THREAD; ++i) {
                std::string key = "thread_" + std::to_string(threadId) + "_" + std::to_string(i);
                
                // 混合操作：设置、查询、移除
                auto expire = std::chrono::system_clock::now() + std::chrono::milliseconds(100);
                ttl.setTTL(key, expire);
                
                ttl.isExpired(key);
                
                if (i % 10 == 0) {
                    ttl.removeTTL(key);
                }
            }
        };
        
        // 启动多个线程
        for (int i = 0; i < THREAD_COUNT; ++i) {
            threads.emplace_back(worker, i);
        }
        
        // 等待所有线程完成
        for (auto& t : threads) {
            t.join();
        }
        
        // 验证没有数据竞争或死锁
        std::cout << "测试6通过：线程安全测试（若无崩溃则通过）\n";
    }
    
    // ===== 测试7：边界条件 =====
    {
        // 空键测试
        ttl.setTTL("", now + std::chrono::seconds(10));
        assert(ttl.isExpired("") == false);
        ttl.removeTTL("");
        
        // 0 TTL测试（立即过期）
        ttl.setTTL("zero_ttl", now);
        assert(ttl.isExpired("zero_ttl") == true);
        
        std::cout << "测试7通过：边界条件测试\n";
    }
    
    std::cout << "\n所有测试用例通过！\n";
}

int main() {
    try {
        runTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "测试失败: " << e.what() << std::endl;
        return 1;
    }
}