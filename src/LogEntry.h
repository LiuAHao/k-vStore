#pragma once
#include <string>
#include <cstdint>

class LogEntry {
public:
    enum class Type {
        SET,
        DELETE,
        NOOP
    };

private:
    Type type_;
    std::string key_;
    std::string value_;
    int64_t requestId_;
    int64_t term_;
    int64_t index_;

public:
    LogEntry() : type_(Type::NOOP), requestId_(0), term_(0), index_(0) {}
    
    LogEntry(Type type, const std::string& key, const std::string& value, 
            int64_t requestId, int64_t term, int64_t index)
        : type_(type), key_(key), value_(value), 
          requestId_(requestId), term_(term), index_(index) {}

    // Getters
    Type getType() const { return type_; }
    int64_t getTerm() const { return term_; }
    int64_t getIndex() const { return index_; }
    std::string getKey() const { return key_; }
    std::string getValue() const { return value_; }
    int64_t getRequestId() const { return requestId_; }

    // Setters
    void setTerm(int64_t term) { term_ = term; }
    void setIndex(int64_t index) { index_ = index; }

    // 添加序列化方法
    std::string serialize() const;
    static LogEntry deserialize(const std::string& data);
};