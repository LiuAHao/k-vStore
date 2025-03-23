#pragma once
#include<string>

class ClientRequest 
{
private:
    std::string key;
    std::string value;
    std::string option;
public:
    ClientRequest();
    ClientRequest(std::string key, std::string value, std::string option);
    ~ClientRequest();
    setKey(std::string key);
    setValue(std::string value);
    setOption(std::string option);
    std::string getKey();
    std::string getValue();
    std::string getOption();
}

