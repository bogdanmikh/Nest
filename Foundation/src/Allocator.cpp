#include "Foundation/Allocator.hpp"

#include <cstdlib>
#include <string>
#include <cstring>

#ifndef CONFIG_ALLOCATOR_NATURAL_ALIGNMENT
#    define CONFIG_ALLOCATOR_NATURAL_ALIGNMENT 8
#endif

void *operator new(size_t, Foundation::PlacementNewTag, void *_ptr) {
    return _ptr;
}

void operator delete(void *, Foundation::PlacementNewTag, void *) throw() {}

namespace Foundation {

template<typename Ty>
inline constexpr Ty max(const Ty &_a, const Ty &_b) {
    return _a > _b ? _a : _b;
}

template<typename Ty>
inline Ty alignUp(Ty _a, int32_t _align) {
    const Ty mask = Ty(_align - 1);
    return Ty((_a + mask) & ~mask);
}

void memMove(void *_dst, const void *_src, size_t _numBytes) {
    ::memmove(_dst, _src, _numBytes);
}

inline void *alignPtr(void *_ptr, size_t _extra, size_t _align) {
    union {
        void *ptr;
        uintptr_t addr;
    } un;
    un.ptr = _ptr;
    uintptr_t unaligned = un.addr + _extra; // space for header
    uintptr_t aligned = alignUp(unaligned, int32_t(_align));
    un.addr = aligned;
    return un.ptr;
}

AllocatorI *getAllocator() {
    static DefaultAllocator allocator;
    return &allocator;
}

void *alloc(AllocatorI *allocator, size_t size, size_t align, const char *file, uint32_t line) {
    return allocator->realloc(nullptr, size, align, file, line);
}

void free(AllocatorI *allocator, void *ptr, size_t align, const char *file, uint32_t line) {
    allocator->realloc(ptr, 0, align, file, line);
}

void *realloc(
    AllocatorI *allocator, void *ptr, size_t size, size_t align, const char *file, uint32_t line
) {
    return allocator->realloc(ptr, size, align, file, line);
}

inline void *alignedAlloc(
    AllocatorI *_allocator, size_t _size, size_t _align, const char *_file, uint32_t _line
) {
    const size_t align = max(_align, sizeof(uint32_t));
    const size_t total = _size + align;
    uint8_t *ptr = (uint8_t *)alloc(_allocator, total, 0, _file, _line);
    uint8_t *aligned = (uint8_t *)alignPtr(ptr, sizeof(uint32_t), align);
    uint32_t *header = (uint32_t *)aligned - 1;
    *header = uint32_t(aligned - ptr);
    return aligned;
}

inline void
alignedFree(AllocatorI *_allocator, void *_ptr, size_t _align, const char *_file, uint32_t _line) {
    uint8_t *aligned = (uint8_t *)_ptr;
    uint32_t *header = (uint32_t *)aligned - 1;
    uint8_t *ptr = aligned - *header;
    free(_allocator, ptr, 0, _file, _line);
}

inline void *alignedRealloc(
    AllocatorI *_allocator,
    void *_ptr,
    size_t _size,
    size_t _align,
    const char *_file,
    uint32_t _line
) {
    if (nullptr == _ptr) {
        return alignedAlloc(_allocator, _size, _align, _file, _line);
    }

    uint8_t *aligned = (uint8_t *)_ptr;
    uint32_t offset = *((uint32_t *)aligned - 1);
    uint8_t *ptr = aligned - offset;

    const size_t align = max(_align, sizeof(uint32_t));
    ;
    const size_t total = _size + align;
    ptr = (uint8_t *)realloc(_allocator, ptr, total, 0, _file, _line);
    uint8_t *newAligned = (uint8_t *)alignPtr(ptr, sizeof(uint32_t), align);

    if (newAligned == aligned) {
        return aligned;
    }

    aligned = ptr + offset;
    memMove(newAligned, aligned, _size);
    uint32_t *header = (uint32_t *)newAligned - 1;
    *header = uint32_t(newAligned - ptr);
    return newAligned;
}

DefaultAllocator::DefaultAllocator() {}

DefaultAllocator::~DefaultAllocator() {}

void *DefaultAllocator::realloc(
    void *_ptr, size_t _size, size_t _align, const char *_file, uint32_t _line
) {
    if (_size == 0) {
        if (_ptr != nullptr) {
            if (_align <= CONFIG_ALLOCATOR_NATURAL_ALIGNMENT) {
                ::free(_ptr);
                return nullptr;
            }

#if COMPILER_MSVC
            _aligned_free(_ptr);
#else
            alignedFree(this, _ptr, _align, _file, _line);
#endif
        }

        return nullptr;
    } else if (_ptr == nullptr) {
        if (_align <= CONFIG_ALLOCATOR_NATURAL_ALIGNMENT) {
            return ::malloc(_size);
        }

#if COMPILER_MSVC
        return _aligned_malloc(_size, _align);
#else
        return alignedAlloc(this, _size, _align, _file, _line);
#endif
    }

    if (_align <= CONFIG_ALLOCATOR_NATURAL_ALIGNMENT) {
        return ::realloc(_ptr, _size);
    }

#if COMPILER_MSVC
    return _aligned_realloc(_ptr, _size, _align);
#else
    return alignedRealloc(this, _ptr, _size, _align, _file, _line);
#endif
}

} // namespace Foundation