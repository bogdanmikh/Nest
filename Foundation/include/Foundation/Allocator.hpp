#pragma once

#include <cstdint>
#include <cstdlib>
#include <set>

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
#    define ALLOC(allocator, size) Foundation::alloc(allocator, size, 0)
#    define FREE(allocator, ptr) Foundation::free(allocator, ptr, 0)
#    define DELETE(_allocator, _ptr)                                                               \
        Foundation::deleteObject(_allocator, _ptr, 0, __FILE__, __LINE__)

#    define ALIGNED_ALLOC(_allocator, _size, _align) Foundation::alloc(_allocator, _size, _align)
#    define ALIGNED_FREE(_allocator, _ptr, _align) Foundation::free(_allocator, _ptr, _align)
#    define ALIGNED_DELETE(_allocator, _ptr, _align)                                               \
        Foundation::deleteObject(_allocator, _ptr, _align)
#endif

#define NEW(_allocator, _type) PLACEMENT_NEW(ALLOC(_allocator, sizeof(_type)), _type)
#define ALIGNED_NEW(_allocator, _type, _align)                                                     \
    PLACEMENT_NEW(ALIGNED_ALLOC(_allocator, sizeof(_type), _align), _type)
#define PLACEMENT_NEW(_ptr, _type) ::new (Foundation::PlacementNewTag(), _ptr) _type

namespace Foundation {
struct PlacementNewTag {};
} // namespace Foundation

void *operator new(size_t, Foundation::PlacementNewTag, void *_ptr);
void operator delete(void *, Foundation::PlacementNewTag, void *) throw();

namespace Foundation {

struct AllocatorI {
    virtual ~AllocatorI() = default;

    virtual void *realloc(void *ptr, size_t size) = 0;
};

class DefaultAllocator : public AllocatorI {
public:
    void *realloc(void *ptr, size_t size) override {
        if (ptr == nullptr) {
            ptr = (void *)new char(size);
            return ptr;
        } else if (size == 0) {
            delete[] ptr;
            return nullptr;
        }
        ptr = realloc(ptr, 0);
        ptr = realloc(ptr, size);
        return ptr;
    }
    inline static DefaultAllocator *getInstance() {
        return s_instance;
    }
    ~DefaultAllocator() override {}

private:
    DefaultAllocator();
    static DefaultAllocator *s_instance;
};

enum MemorySize { KILOBYTE = 1000, MEGABYTE = KILOBYTE * 1000, GIGABYTE = MEGABYTE * 1000 };

class FreeListAllocator : public AllocatorI {
public:
    void *realloc(void *ptr, size_t size) override;
    void getInfo();
    inline static FreeListAllocator *getInstance() {
        return s_instance;
    }
    ~FreeListAllocator() override;

private:
    struct Block {
        Block(size_t size, Block *nextBlocOccupied = nullptr, size_t offsetAtStart = 0)
            : blockSize(size)
            , next(nextBlocOccupied)
            , offset(offsetAtStart) {}

        size_t blockSize;
        Block *next;
        size_t offset;

        bool operator<(const Block &other) const {
            return offset < other.offset;
        };

        bool operator>(const Block &other) const {
            return offset > other.offset;
        };

        bool operator==(const Block &other) const {
            return offset == other.offset;
        };

        bool operator!=(const Block &other) const {
            return offset != other.offset;
        };

        bool operator<=(const Block &other) const {
            return offset <= other.offset;
        };

        bool operator>=(const Block &other) const {
            return offset >= other.offset;
        };
    };
    explicit FreeListAllocator(size_t totalSize);
    void combine(std::set<Block>::iterator newFreeBlockIt);
    void reset();
    void printAllBlocks();
    void *m_memory;
    static FreeListAllocator *s_instance;
    std::set<Block> m_freeBlocks;
    std::set<Block> m_occupiedBlocks;
    size_t m_totalSize;
    size_t m_used;
};

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