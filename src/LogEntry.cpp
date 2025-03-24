#include "LogEntry.h"
#include <json/json.h>

std::string LogEntry::serialize() const {
    Json::Value root;
    root["type"] = static_cast<int>(type_);
    root["key"] = key_;
    root["value"] = value_;
    root["requestId"] = Json::Int64(requestId_);
    root["term"] = Json::Int64(term_);
    root["index"] = Json::Int64(index_);
    
    Json::FastWriter writer;
    return writer.write(root);
}

LogEntry LogEntry::deserialize(const std::string& data) {
    Json::Value root;
    Json::Reader reader;
    reader.parse(data, root);
    
    return LogEntry(
        static_cast<Type>(root["type"].asInt()),
        root["key"].asString(),
        root["value"].asString(),
        root["requestId"].asInt64(),
        root["term"].asInt64(),
        root["index"].asInt64()
    );
}
