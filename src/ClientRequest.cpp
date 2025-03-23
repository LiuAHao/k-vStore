#include "ClientRequest.h"

ClientRequest::ClientRequest()
    : key(""), value(""), option("") {
}

ClientRequest::ClientRequest(const std::string& key, const std::string& value)
    : key(key), value(value), option("") {
}

ClientRequest::ClientRequest(const std::string& key, const std::string& value, const std::string& option)
    : key(key), value(value), option(option) {
}

void ClientRequest::setKey(const std::string& key) {
    this->key = key;
}

void ClientRequest::setValue(const std::string& value) {
    this->value = value;
}

void ClientRequest::setOption(const std::string& option) {
    this->option = option;
}

std::string ClientRequest::getKey() const {
    return key;
}

std::string ClientRequest::getValue() const {
    return value;
}

std::string ClientRequest::getOption() const {
    return option;
}

