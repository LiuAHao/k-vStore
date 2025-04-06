#pragma once
#include <cstring>

struct Value {
    void* data = nullptr;
    size_t size = 0;

    Value() = default;

    // 拷贝构造函数
    Value(const Value& other) : data(nullptr), size(0) {
        if (other.data && other.size > 0) {
            size = other.size;
            data = other.data;  // 直接使用指针，因为内存由 SlabAllocator 管理
        }
    }

    // 赋值运算符
    Value& operator=(const Value& other) {
        if (this != &other) {
            data = other.data;  // 直接使用指针，因为内存由 SlabAllocator 管理
            size = other.size;
        }
        return *this;
    }

    bool isNull() const {
        return data == nullptr || size == 0;
    }

    // 添加安全访问字符串的方法
    std::string toString() const {
        if (isNull()) return "";
        return std::string(static_cast<const char*>(data), size);
    }


    // 析构函数不释放内存，因为内存由 SlabAllocator 管理
    ~Value() = default;
};