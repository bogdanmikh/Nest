#pragma once

#include <cstdint>
#include <cstdlib>

#if ALLOCATOR_DEBUG
#    define ALLOC(allocator, size) Foundation::alloc(allocator, _size, 0, __FILE__, __LINE__)
#    define FREE(allocator, ptr) Foundation::free(allocator, ptr, 0, __FILE__, __LINE__)
#    define DELETE(_allocator, _ptr) Foundation::deleteObject(_allocator, _ptr)

#    define ALIGNED_ALLOC(_allocator, _size, _align)                                               \
        Foundation::alloc(_allocator, _size, _align, __FILE__, __LINE__)
#    define ALIGNED_FREE(_allocator, _ptr, _align)                                                 \
        Foundation::free(_allocator, _ptr, _align, __FILE__, __LINE__)
#    define ALIGNED_DELETE(_allocator, _ptr, _align)                                               \
        Foundation::deleteObject(_allocator, _ptr, _align, __FILE__, __LINE__)
#else
#    define F_ALLOC(allocator, size) Foundation::alloc(allocator, size, 0)
#    define F_FREE(allocator, ptr) Foundation::free(allocator, ptr, 0)
#    define F_DELETE(_allocator, _ptr)                                                             \
        Foundation::deleteObject(_allocator, _ptr, 0, __FILE__, __LINE__)

#    define F_ALIGNED_ALLOC(_allocator, _size, _align) Foundation::alloc(_allocator, _size, _align)
#    define F_ALIGNED_FREE(_allocator, _ptr, _align) Foundation::free(_allocator, _ptr, _align)
#    define F_ALIGNED_DELETE(_allocator, _ptr, _align)                                             \
        Foundation::deleteObject(_allocator, _ptr, _align)
#endif

#define F_NEW(_allocator, _type) F_PLACEMENT_NEW(F_ALLOC(_allocator, sizeof(_type)), _type)
#define F_ALIGNED_NEW(_allocator, _type, _align)                                                   \
    F_PLACEMENT_NEW(F_ALIGNED_ALLOC(_allocator, sizeof(_type), _align), _type)
#define F_PLACEMENT_NEW(_ptr, _type) ::new (Foundation::PlacementNewTag(), _ptr) _type

namespace Foundation {
struct PlacementNewTag {};
} // namespace Foundation

void *operator new(size_t, Foundation::PlacementNewTag, void *_ptr);
void operator delete(void *, Foundation::PlacementNewTag, void *) throw();

namespace Foundation {

struct AllocatorI {
    virtual ~AllocatorI() = default;

    virtual void *
    realloc(void *ptr, size_t size, size_t _align, const char *file, uint32_t line) = 0;
};

class DefaultAllocator : public AllocatorI {
public:
    DefaultAllocator();

    ~DefaultAllocator() override;

    void *realloc(void *ptr, size_t size, size_t _align, const char *file, uint32_t line) override;
};

template<typename Ty>
Ty alignUp(Ty _a, int32_t _align);

void *alloc(
    AllocatorI *allocator,
    size_t size,
    size_t _align = 0,
    const char *file = nullptr,
    uint32_t line = 0
);

void free(
    AllocatorI *allocator,
    void *ptr,
    size_t _align = 0,
    const char *file = nullptr,
    uint32_t line = 0
);

template<typename ObjectT>
inline void deleteObject(
    AllocatorI *_allocator,
    ObjectT *_object,
    size_t _align = 0,
    const char *_file = nullptr,
    uint32_t _line = 0
) {
    if (_object == nullptr) {
        return;
    }
    _object->~ObjectT();
    free(_allocator, _object, _align, _file, _line);
}

AllocatorI *getAllocator();

} // namespace Foundation