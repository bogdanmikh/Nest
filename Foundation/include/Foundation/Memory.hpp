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

    const void release() {
        if (!data) {
            return;
        }
        if (releaseFn != nullptr) {
            releaseFn(data, userData);
        } else if (data != nullptr) {
            F_FREE(Foundation::getAllocator(), data);
        }
        data = nullptr;
    }

    /// Create memory copying some data
    static Memory copying(void *src, uint32_t size) {
        void *data = F_ALLOC(Foundation::getAllocator(), size);
        memcpy(data, src, size);
        return Memory(data, nullptr, [](void *ptr, void *) {
            F_FREE(Foundation::getAllocator(), ptr);
        });
    }

    static Memory alloc(uint32_t size) {
        void *data = F_ALLOC(Foundation::getAllocator(), size);
        return Memory(data, nullptr, [](void *ptr, void *) {
            F_FREE(Foundation::getAllocator(), ptr);
        });
    }
};

} // namespace Foundation