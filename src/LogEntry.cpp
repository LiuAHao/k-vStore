#include "LogEntry.h"

LogEntry::LogEntry(Type type, const std::string& key, const std::string& value, int64_t requestId, int64_t term, int64_t index)
    : type(type), key(key), value(value), requestId(requestId), term(term), index(index) {}

std::string LogEntry::serialize() const {
    return to_string(type) + "|" + key + "|" + value + "|" + to_string(requestId) + "|" + to_string(term) + "|" + to_string(index);
}

LogEntry LogEntry::deserialize(const std::string& data) {
    std::vector<std::string> parts = split(data, "|");
    return LogEntry(static_cast<Type>(stoi(parts[0])), parts[1], parts[2], stoi(parts[3]), stoi(parts[4]), stoi(parts[5]));
}