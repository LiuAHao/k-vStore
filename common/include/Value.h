#pragma once
#include <new>
struct Value {
    void* data;
    size_t size;
    
    ~Value() {
        if (data) {
            free(data); // 确保释放内存
        }
    }
};