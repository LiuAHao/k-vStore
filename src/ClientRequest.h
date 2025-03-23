#pragma once
#include <string>

class ClientRequest {
private:
    std::string key;
    std::string value;
    std::string option;

public:
    // 构造函数
    ClientRequest();
    ClientRequest(const std::string& key, const std::string& value);
    ClientRequest(const std::string& key, const std::string& value, const std::string& option);
    
    // setter 方法
    void setKey(const std::string& key);
    void setValue(const std::string& value);
    void setOption(const std::string& option);
    
    // getter 方法
    std::string getKey() const;
    std::string getValue() const;
    std::string getOption() const;
};

