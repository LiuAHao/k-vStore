#include"../include/SlabAllocator.h"


SlabAllocator::~SlabAllocator(){

}


void* SlabAllocator::allocate(size_t size){
    if(size == 0 || size > max_memory_)return nullptr;

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
    used_memory_ += sc.item_size;

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
    used_memory_ -= sc.item_size;
}

size_t SlabAllocator::getUsedMemory() const{
    std::lock_guard<std::mutex> lock(mutex_);
    return used_memory_;
}
size_t SlabAllocator::getFreeMemory() const{
    std::lock_guard<std::mutex> lock(mutex_);
    return max_memory_ - used_memory_;
}

size_t SlabAllocator::findSlabClass(size_t size){
    //返回第一个大于它的内存块
    for(size_t i = 0; i < slab_classes_.size(); i++){
        if(slab_classes_[i].item_size > size){
            return i;
        }
    }

    //如果没有，就创建新的 判断是否为空，空则创建相同大小块，非空则以增长因子增加
    size_t new_size = slab_classes_.empty()?size:
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

    return slab_classes_.size() - 1;

}

void* SlabAllocator::allocateNewSlap(size_t class_index){
    if(class_index > slab_classes_.size()){
        return nullptr;
    }

    SlabClass& sc = slab_classes_[class_index];
    size_t slab_size = SLAB_SIZE;
    
    if(used_memory_ + slab_size > max_memory_){
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

    //更新已用内存统计
    used_memory_ += slab_size;
    //返回新建内存首地址
    return new_slab;
}
