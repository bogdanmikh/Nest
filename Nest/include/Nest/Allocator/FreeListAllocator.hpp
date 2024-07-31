#pragma once
#include <set>
#include "AllocatorInterface.hpp"

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

struct PlacementNewTag {};

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
