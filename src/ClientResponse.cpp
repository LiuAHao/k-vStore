#include "ClientResponse.h"

ClientResponse::ClientResponse(bool success, const std::string& responseValue, bool error)
    : isSuccess(success), value(responseValue), hasError(error) {
    // 构造函数实现
}

bool ClientResponse::success() const {
    return isSuccess;
}

bool ClientResponse::error() const {
    return hasError;
}

std::string ClientResponse::getValue() const {
    return value;
}
