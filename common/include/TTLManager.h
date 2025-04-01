#pragma once
#include <chrono>
#include <unordered_map>
#include <string>
#include <mutex>
#include <vector>

/*
TTL过期机制实现
    TTL(Time To Live)机制确保数据不会无限期驻留内存，通常有两种实现方式：
    1. 被动检查方式
        每次访问对象时检查是否过期
    2. 主动清理方式
        通过后台线程定期扫描并清理过期对象
*/
class TTLManager {
public:
    using TimePoint = std::chrono::system_clock::time_point;

    static TimePoint msToimePoint(long long ttl_ms){
        return std::chrono::system_clock::now() + std::chrono::milliseconds(ttl_ms);
    }
    // 设置键的TTL
    void setTTL(const std::string& key, TimePoint expire_time);
    // 获取键的剩余生存时间(毫秒)
    long long get_TTL(const std::string& key)const;
    // 检查键是否过期
    bool isExpired(const std::string& key)const;
    // 删除键的TTL记录
    void removeTTL(const std::string& key);
    // 主动清理所有过期键
    std::vector<std::string> cleanupExpired();

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, TimePoint> TTL_map_;
};