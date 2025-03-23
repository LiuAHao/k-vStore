#pragma once
#include <string>

class ClientResponse {
private:
    bool isSuccess;  // 改名避免与函数冲突
    std::string value;
    bool hasError;   // 改名避免与函数冲突

public:
    // 修复构造函数参数默认值
    ClientResponse(bool success = false, const std::string& responseValue = "", bool error = false);
    
    // 获取响应状态的方法
    bool success() const;     // 成功返回 true
    bool error() const;       // 错误返回 true
    std::string getValue() const;  // 获取响应值
};