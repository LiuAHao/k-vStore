#pragma once
#include <cstddef>
#include <vector>
#include <list>
#include <unordered_map>
#include <mutex>
#include <iostream>

/*
Slab内存分配机制
    Slab分配器是一种高效的内存管理机制，主要用于减少内存碎片和提高分配效率。其核心思想是预分配不同大小的内存块，每个Slab管理相同大小的对象。
SlabAllocator (整个内存池)
│
├── SlabClass[0] (管理32字节对象)
│   ├── SlabPage1 (1MB内存块)
│   │   ├── 槽0 (32字节)
│   │   ├── 槽1 (32字节)
│   │   └── ... (共32,768个槽)
│   └── SlabPage2
│       └── ...
│
├── SlabClass[1] (管理64字节对象)
│   ├── SlabPage1 (1MB内存块)
│   │   ├── 槽0 (64字节)
│   │   ├── 槽1 (64字节)
│   │   └── ... (共16,384个槽)
│   └── ...
└── ...
Slab工作原理
    分级预分配：系统预先创建多个Slab类，每个类负责特定大小(slab页)的内存分配

    对象缓存：每个Slab包含多个相同大小的"槽"(chunk)，用于存储对象
    空闲列表：维护空闲对象的链表，快速分配和回收
*/

class SlabAllocator {
public:
    // 初始化Slab系统，指定最大内存和增长因子
    explicit SlabAllocator(size_t max_memory, float growth_factor = 2);
    ~SlabAllocator();
    
    // 分配指定大小的内存
    void* allocate(size_t size);
    
    // 释放内存
    void deallocate(void* ptr);
    
    // 获取内存使用统计
    size_t getUsedMemory() const;
    size_t getFreeMemory() const;
    size_t getMaxMemory() const;
    size_t getAllocatedSlabMemory() const;

private:
    struct SlabClass{
        size_t item_size;    //该类管理的对象（槽）大小
        size_t item_per_slab;   //每个slab页能存放的对象（槽）数
        std::list<void*> free_list; //空闲对象（槽）列表
    };

    // 根据请求大小找到合适的Slab类
    size_t findSlabClass(size_t size);
    // 在指定的SlabClass中新增一个Slab页（1MB），相当于给这个SlabClass增加了一组可用的槽
    void* allocateNewSlap(size_t class_index);
    // 所有Slab类
    std::vector<SlabClass> slab_classes_;
    // 指针到Slab类的映射
    std::unordered_map<void*, size_t> ptr_to_class_;
    // 新增：记录所有分配的Slab基地址
    std::vector<void*> slab_pages_;

    size_t max_memory_;
    const float growth_factor_;
    size_t allocated_slab_memory_;  // 已分配的Slab内存总和
    size_t used_object_memory_;     // 实际使用的对象内存总和
    
    // 1MB的Slab页
    static constexpr size_t SLAB_SIZE = 1024 * 1024; 
    mutable std::mutex mutex_;

    // 添加新方法声明
    void tryReleaseEmptySlab(size_t class_index);
};