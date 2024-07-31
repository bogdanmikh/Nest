#pragma once
#include <cstddef>

struct AllocatorI {
    virtual ~AllocatorI() = default;
    virtual void *realloc(void *ptr, size_t size) = 0;
};
