#pragma once

#include <cstdlib>
#include <cstring>
#include "Allocator.hpp"

using size = uint32_t;

struct Memory {
    Memory(void *data = nullptr)
        : data(data) {}

    void *data;

    const void release() const {
        FREE(data);
    }

    /// Create memory copying some data
    /* static Memory copying(void *src, uint32_t size) {
         void *data = ALLOC(Foundation::getAllocator(), size);
         memcpy(data, src, size);
     } */

    static Memory alloc(uint32_t size) {
        void *data = ALLOC_ARRAY(char, size);
        return data;
    }
};