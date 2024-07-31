#pragma once

#include "FreeListAllocator.hpp"

FreeListAllocator *FreeListAllocator::s_instance = new FreeListAllocator(MemorySize::MEGABYTE);

struct Allocator : public AllocatorI {
public:
    static AllocatorI *get() {
        return allocator;
    }
    void *realloc(void *ptr, size_t size) override {
        return allocator->realloc(ptr, size);
    }
    ~Allocator() = 0;

private:
    static AllocatorI *allocator;
};

AllocatorI *Allocator::allocator = FreeListAllocator::getInstance();

#define NEW(_type) Allocator::get()->realloc(nullptr, sizeof(_type))
#define NEW_ARRAY(_type, _size) Allocator::get()->realloc(nullptr, sizeof(_type) * _size)
#define FREE(_ptr) Allocator::get()->realloc(_ptr, 0)