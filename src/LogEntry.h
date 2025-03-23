#pragma once
#include <string>

class LogEntry 
{
public:
    enum class Type { SET, GET, DELETE, NOOP };
    Type type;
    std::string key;    // 操作的键（GET/SET/DELETE 时使用）
    std::string value;  // 操作的值（仅 SET 时使用）
    int64_t requestId;  // 客户端请求唯一ID（用于幂等性）
    int64_t term;       // 日志对应的任期号（Raft 关键字段）
    int64_t index;      // 日志在全局的顺序索引
    LogEntry(Type type, const std::string& key, const std::string& value, int64_t requestId, int64_t term, int64_t index);
    std::string serialize() const;
    static LogEntry deserialize(const std::string& data);
    Type getOp() const { return op; }
    int getTerm() const { return term; }
    int getIndex() const { return index; }
    std::string getKey() const { return key; }
    std::string getValue() const { return value; }
    int64_t getRequestId() const { return requestId; }
};