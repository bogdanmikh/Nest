#pragma once

#include <cstdlib>
#include <Foundation/Allocator.hpp>

namespace Foundation {

using ReleaseFunction = void (*)(void *ptr, void *userData);

struct Memory {
    Memory(void *data = nullptr, void *userData = nullptr, ReleaseFunction releaseFun = nullptr)
        : data(data)
        , userData(userData)
        , releaseFn(releaseFun) {}

    void *data;
    void *userData;
    ReleaseFunction releaseFn;

    const void release() const {
        if (releaseFn != nullptr) {
            releaseFn(data, userData);
        } else if (data != nullptr) {
            FREE(Foundation::getAllocator(), data);
        }
    }

    /// Create memory copying some data
    static Memory copying(void *src, uint32_t size) {
        void *data = ALLOC(Foundation::getAllocator(), size);
        memcpy(data, src, size);
        return Memory(data, nullptr, [](void *ptr, void *) {
            FREE(Foundation::getAllocator(), ptr);
        });
    }

    static Memory alloc(uint32_t size) {
        void *data = ALLOC(Foundation::getAllocator(), size);
        return Memory(data, nullptr, [](void *ptr, void *) {
            FREE(Foundation::getAllocator(), ptr);
        });
    }
};

} // namespace Foundation