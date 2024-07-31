#pragma once

#include "FreeListAllocator.hpp"

FreeListAllocator *FreeListAllocator::s_instance = new FreeListAllocator(MemorySize::MEGABYTE);

struct Allocator {
public:
    static AllocatorI* get() {
        return allocator;
    }
private:
    static AllocatorI *allocator;
};

AllocatorI *Allocator::allocator = FreeListAllocator::getInstance();

#define ALLOC(_type) Allocator::get()->realloc(nullptr, sizeof(_type))
#define ALLOC_ARRAY(_type, _size) Allocator::get()->realloc(nullptr, sizeof(_type) * _size)
#define FREE(_ptr) Allocator::get()->realloc(_ptr, 0)