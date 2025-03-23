#include "LogEntry.h"
#include <vector>
#include <sstream>

// 辅助函数：分割字符串
std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    size_t prev = 0;
    while ((pos = s.find(delimiter, prev)) != std::string::npos) {
        tokens.push_back(s.substr(prev, pos - prev));
        prev = pos + delimiter.length();
    }
    tokens.push_back(s.substr(prev));
    return tokens;
}

LogEntry::LogEntry(Type type, const std::string& key, const std::string& value, int64_t requestId, int64_t term, int64_t index)
    : type(type), key(key), value(value), requestId(requestId), term(term), index(index) 
{}

std::string LogEntry::serialize() const 
{
    return std::to_string(static_cast<int>(type)) + "|" + key + "|" + value + "|" + 
           std::to_string(requestId) + "|" + std::to_string(term) + "|" + std::to_string(index);
}

LogEntry LogEntry::deserialize(const std::string& data) 
{
    std::vector<std::string> parts = split(data, "|");
    if (parts.size() != 6) {
        throw std::runtime_error("Invalid log entry format");
    }
    return LogEntry(
        static_cast<Type>(std::stoi(parts[0])), 
        parts[1], 
        parts[2], 
        std::stoll(parts[3]), 
        std::stoll(parts[4]), 
        std::stoll(parts[5])
    );
}
