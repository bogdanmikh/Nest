#pragma once
#include <set>
#include "AllocatorInterface.hpp"

class DefaultAllocator: public AllocatorI {
public:
    void *realloc(void *ptr, size_t size) override {
        if (ptr == nullptr) {
            ptr = (void*)new char(size);
            return ptr;
        } else if (size == 0) {
            delete[] ptr;
            return nullptr;
        }
        ptr = realloc(ptr, 0);
        ptr = realloc(ptr, size);
        return ptr;
    }
    inline static DefaultAllocator* getInstance() {
        return s_instance;
    }
    ~DefaultAllocator() override {}
private:
    DefaultAllocator();
    static DefaultAllocator* s_instance;
};

/*
void* operator new(size_t size, PlacementNewTag) {
    return FreeListAllocator::getInstance()->realloc(nullptr, size);
}

void* operator new[](size_t size, PlacementNewTag) {
    return FreeListAllocator::getInstance()->realloc(nullptr, size);
}

void operator delete(void* ptr, PlacementNewTag) {
    FreeListAllocator::getInstance()->realloc(ptr, 0);
}

void operator delete[](void* ptr, PlacementNewTag) {
    FreeListAllocator::getInstance()->realloc(ptr, 0);
}

*/

