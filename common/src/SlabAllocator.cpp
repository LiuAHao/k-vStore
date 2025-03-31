#include"../include/SlabAllocator.h"


SlabAllocator::SlabAllocator(size_t max_memory, float growth_factor) 
    : max_memory_(max_memory), 
      growth_factor_(growth_factor),
      allocated_slab_memory_(0),
      used_object_memory_(0){
}

SlabAllocator::~SlabAllocator(){
    std::lock_guard<std::mutex> lock(mutex_);
    for(void* page:slab_pages_){
        free(page);
    }
}


void* SlabAllocator::allocate(size_t size){
    //如果分配的内存大于页大小也视为错误
    if(size == 0 || size > max_memory_ || size > SLAB_SIZE)return nullptr;

    size_t class_index = findSlabClass(size);
    SlabClass& sc = slab_classes_[class_index];

    std::lock_guard<std::mutex> lock(mutex_);

    if(sc.free_list.empty()){
        void* new_slab = allocateNewSlap(class_index);
        if(!new_slab) return nullptr;
    }
    void* ptr = sc.free_list.front();
    sc.free_list.pop_front();
    ptr_to_class_[ptr] = class_index;
    used_object_memory_ += sc.item_size;

    return ptr;
}


void SlabAllocator::deallocate(void* ptr){
    if(!ptr)return;

    std::lock_guard<std::mutex> lock(mutex_);

    auto it = ptr_to_class_.find(ptr);
    if(it == ptr_to_class_.end()){
        //未知非法指针
        return;
    }
    size_t class_index = it->second;
    SlabClass& sc = slab_classes_[class_index];
    //把空闲块放回空闲列表
    sc.free_list.push_front(ptr);
    ptr_to_class_.erase(it);
    used_object_memory_ -= sc.item_size;
}

size_t SlabAllocator::getUsedMemory() const{
    std::lock_guard<std::mutex> lock(mutex_);
    return used_object_memory_;
}

size_t SlabAllocator::getAllocatedSlabMemory() const{
    std::lock_guard<std::mutex> lock(mutex_);
    return allocated_slab_memory_;
}


size_t SlabAllocator::getFreeMemory() const{
    std::lock_guard<std::mutex> lock(mutex_);
    return allocated_slab_memory_ - used_object_memory_;
}

size_t SlabAllocator::getMaxMemory() const{
    std::lock_guard<std::mutex> lock(mutex_);
    return max_memory_;
}


size_t SlabAllocator::findSlabClass(size_t size){
    //返回第一个大于它的内存块
    for(size_t i = 0; i < slab_classes_.size(); i++){
        if(slab_classes_[i].item_size >= size){
            if(slab_classes_[i].free_list.empty()){
                allocateNewSlap(i);
            }
            return i;
        }
    }
    size_t new_size;
    do{
        //如果没有，就创建新的 判断是否为空，空则创建相同大小块，非空则以增长因子增加
        new_size = slab_classes_.empty()?size:
        slab_classes_.back().item_size * growth_factor_;

        // 确保不超过最大内存限制
        if(new_size > max_memory_){
            //抛出异常
            throw std::bad_alloc();     
        }

        const_cast<SlabAllocator*>(this)->slab_classes_.push_back({
            new_size, 
            SLAB_SIZE / new_size,
            {}
        });
    }while(new_size < size);

    return slab_classes_.size() - 1;
}

void* SlabAllocator::allocateNewSlap(size_t class_index){
    
    if(class_index > slab_classes_.size()){
        return nullptr;
    }

    SlabClass& sc = slab_classes_[class_index];
    size_t slab_size = SLAB_SIZE;
    
    if(allocated_slab_memory_ + slab_size > max_memory_){
        std::cout << "分配总内存超过最大容量" << std::endl;
        return nullptr;
    }

    // 分配新的Slab页
    void* new_slab = ::malloc(slab_size);
    if (!new_slab) {
        return nullptr;
    }

    //分割这个大内存块为多个小槽,将void转为char是为了按字节计算偏移量
    char* current = static_cast<char*> (new_slab);
    for(size_t i = 0; i < sc.item_per_slab; i++){
        //将每个槽的起始地址加入空闲列表
        sc.free_list.push_back(current);
        //记录这个槽属于哪个SlabClass
        ptr_to_class_[current] = class_index;
        //移动到下一个槽的位置
        current += sc.item_size;
    }

    slab_pages_.push_back(new_slab);
    //更新已经分配内存统计
    allocated_slab_memory_ += slab_size;
    //返回新建内存首地址
    return new_slab;
}

//测试函数
#include <iostream>
#include <vector>
#include <thread>
#include <cassert>
#include "../include/SlabAllocator.h"

void basicTest() {
    std::cout << "=== 开始基础测试 ===" << std::endl;
    
    // 创建1MB内存池
    SlabAllocator allocator(10 * 1024 * 1024);
    
    // 测试1: 分配小块内存
    void* ptr1 = allocator.allocate(128);
    assert(ptr1 != nullptr);
    std::cout << "测试1通过: 128字节分配成功" << std::endl;
    
    // 测试2: 分配较大块内存
    void* ptr2 = allocator.allocate(1024);
    assert(ptr2 != nullptr);
    std::cout << "测试2通过: 1024字节分配成功" << std::endl;
    
    // 测试3: 内存统计
    std::cout << "已用内存: " << allocator.getUsedMemory() << "字节" << std::endl;
    std::cout << "剩余内存: " << allocator.getFreeMemory() << "字节" << std::endl;
    assert(allocator.getUsedMemory() == 128 + 1024);
    
    // 测试4: 释放内存
    allocator.deallocate(ptr1);
    allocator.deallocate(ptr2);
    assert(allocator.getUsedMemory() == 0);
    std::cout << "测试4通过: 内存释放成功" << std::endl;
    
    // 测试5: 分配超过最大限制
    void* ptr3 = allocator.allocate(2 * 1024 * 1024);
    assert(ptr3 == nullptr);
    std::cout << "测试5通过: 超大分配返回nullptr" << std::endl;
    
    std::cout << "=== 基础测试通过 ===" << std::endl << std::endl;
}

void slabClassTest() {
    std::cout << "=== 开始Slab类测试 ===" << std::endl;
    
    SlabAllocator allocator(2 * 1024 * 1024);
    
    // 测试Slab类自动创建
    void* ptr1 = allocator.allocate(200);
    assert(ptr1 != nullptr);
    
    // 应该创建了200 * growth_factor大小的Slab类
    void* ptr2 = allocator.allocate(300);
    assert(ptr2 != nullptr);
    
    std::cout << "Slab类自动创建测试通过" << std::endl;
    
    // 验证不同大小分配到不同Slab类
    assert(allocator.getUsedMemory() == 200 + 400);
    
    allocator.deallocate(ptr1);
    allocator.deallocate(ptr2);
    
    std::cout << "=== Slab类测试通过 ===" << std::endl << std::endl;
}

void concurrentTest() {
    std::cout << "=== 开始并发测试 ===" << std::endl;
    
    SlabAllocator allocator(1024 * 1024);
    std::vector<std::thread> threads;
    std::vector<void*> pointers;
    
    const int THREAD_COUNT = 10;
    const int ALLOC_PER_THREAD = 100;
    
    // 多线程分配
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threads.emplace_back([&]() {
            std::vector<void*> local_ptrs;
            for (int j = 0; j < ALLOC_PER_THREAD; ++j) {
                void* ptr = allocator.allocate(64);
                assert(ptr != nullptr);
                local_ptrs.push_back(ptr);
            }
            
            // 确保释放时也加锁
            for (void* ptr : local_ptrs) {
                allocator.deallocate(ptr);
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    // 最终内存应该完全释放
    assert(allocator.getUsedMemory() == 0);
    std::cout << "并发测试通过 (无死锁和数据竞争)" << std::endl;
    
    std::cout << "=== 并发测试通过 ===" << std::endl << std::endl;
}

void edgeCaseTest() {
    std::cout << "=== 开始边界条件测试 ===" << std::endl;
    
    // 测试1: 零字节分配
    {
        SlabAllocator allocator(1024);
        void* ptr = allocator.allocate(0);
        assert(ptr == nullptr);
        std::cout << "测试1通过: 零字节分配返回nullptr" << std::endl;
    }
    
    // 测试2: 重复释放
    {
        SlabAllocator allocator(1024);
        void* ptr = allocator.allocate(64);
        allocator.deallocate(ptr);
        allocator.deallocate(ptr); // 应该安全处理
        std::cout << "测试2通过: 重复释放不会崩溃" << std::endl;
    }
    
    // 测试3: 非法指针释放
    {
        SlabAllocator allocator(1024);
        int dummy;
        allocator.deallocate(&dummy); // 非分配指针
        std::cout << "测试3通过: 非法指针释放不会崩溃" << std::endl;
    }
    
    std::cout << "=== 边界条件测试通过 ===" << std::endl << std::endl;
}

int main() {
    try {
        basicTest();
        slabClassTest();
        concurrentTest();
        edgeCaseTest();
        
        std::cout << "所有测试通过！SlabAllocator 实现正确。" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "测试失败: " << e.what() << std::endl;
        return 1;
    }
}
