#include "ClientRequest.h"

ClientRequest::ClientRequest() 
{}

ClientRequest::ClientRequest(const std::string& key, const std::string& value)
    : key(key), value(value) 
    {}

ClientRequest::~ClientRequest() 
{}

void ClientRequest::setKey(const std::string& key) 
{
    this->key = key;
}

void ClientRequest::setValue(const std::string& value) 
{
    this->value = value;
}
void setOption(const std::string& option) 
{
    this->option = option;
}

std::string ClientRequest::getKey() 
{
    return this->key;
}

std::string ClientRequest::getValue() 
{
    return this->value;
}

std::string ClientRequest::getOption()
{
    return this->option;
}

