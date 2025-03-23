#pragma once
#include <string>

class ClientResponse 
{
private:
    bool success;
    std::string value;
    bool error;
public:
    
    ClientResponse(bool success = false, const std::string& value, bool error = false);
    ~ClientResponse();
    bool success();     // 成功返回 true
    bool error();       // 错误返回 true

}