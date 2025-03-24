#pragma once
#include <stdexcept>

class RaftException : public std::runtime_error {
public:
    explicit RaftException(const std::string& message) 
        : std::runtime_error(message) {}
};