#include "Foundation/Allocator.hpp"
#include "Foundation/Assert.hpp"
#include "Foundation/Logger.hpp"

#include <memory>

#ifndef CONFIG_ALLOCATOR_NATURAL_ALIGNMENT
#    define CONFIG_ALLOCATOR_NATURAL_ALIGNMENT 8
#endif

void *operator new(size_t, Foundation::PlacementNewTag, void *_ptr) {
    return _ptr;
}

void operator delete(void *, Foundation::PlacementNewTag, void *) throw() {}

namespace Foundation {

AllocatorI *getAllocator() {
    static DefaultAllocator allocator;
    //    static FreeListAllocator allocator(MemorySize::MEGABYTE * 10);
    return &allocator;
}

void *alloc(AllocatorI *allocator, size_t size) {
    return allocator->realloc(nullptr, size);
}

void free(AllocatorI *allocator, void *ptr) {
    allocator->realloc(ptr, 0);
}

void *realloc(AllocatorI *allocator, void *ptr, size_t size) {
    return allocator->realloc(ptr, size);
}

FreeListAllocator::FreeListAllocator(size_t totalSize)
    : m_totalSize(totalSize) {
    //    LOG_INFO("Allocator createdLOG_INFO");
    m_memory = std::malloc(m_totalSize);
    reset();
}

void *FreeListAllocator::realloc(void *ptr, size_t size) {
    if (ptr == nullptr) {
        NEST_ASSERT(size < m_totalSize - m_used, "Allocation size must be bigger");
        auto freeBlockIt =
            std::find_if(m_freeBlocks.begin(), m_freeBlocks.end(), [size](const Block &b) {
                return size <= b.blockSize;
            });
        if (freeBlockIt == m_freeBlocks.end()) {
            throw std::bad_alloc();
        }
        size_t offset = freeBlockIt->offset;
        auto &freeBlock = (Block &)*freeBlockIt;
        ptr = static_cast<char *>(m_memory) + freeBlock.offset;
        //        LOG_INFO("Allocated from address: {}, size: {}, offset {}", ptr, size,
        //        freeBlock.offset);
        freeBlock.blockSize -= size;
        freeBlock.offset += size;
        m_used += size;
        auto occupiedFrontBlockIt = m_occupiedBlocks.lower_bound(offset);
        Block newOccupiedBlock(size);
        newOccupiedBlock.offset = offset;
        if (occupiedFrontBlockIt != m_occupiedBlocks.end()) {
            auto &occupiedFrontBlock = (Block &)*occupiedFrontBlockIt;
            newOccupiedBlock.next = reinterpret_cast<Block *>(&occupiedFrontBlock);
        }
        if (occupiedFrontBlockIt != m_occupiedBlocks.begin()) {
            auto &occupiedPrevBlock = (Block &)*std::prev(occupiedFrontBlockIt);
            occupiedPrevBlock.next = reinterpret_cast<Block *>(&newOccupiedBlock);
        }
        m_occupiedBlocks.insert(newOccupiedBlock);
        return ptr;
    } else if (size == 0) {
        // get offset at pointer
        size_t offset = static_cast<char *>(ptr) - static_cast<char *>(m_memory);
        NEST_ASSERT(offset >= 0 && offset <= m_totalSize, "Pointer exit for borders");
        auto occupiedBlockIt = std::find_if(
            m_occupiedBlocks.begin(),
            m_occupiedBlocks.end(),
            [offset](const Block &b) { return offset == b.offset; }
        );
        Block newFreeBlock(occupiedBlockIt->blockSize, occupiedBlockIt->next, offset);
        m_used -= newFreeBlock.blockSize;
        m_occupiedBlocks.erase(occupiedBlockIt);
        auto newFreeBlockIt = m_freeBlocks.insert(newFreeBlock).first;
        combine(newFreeBlockIt);
        return nullptr;
    }
    ptr = realloc(ptr, 0);
    ptr = realloc(ptr, size);
    return ptr;
}

void FreeListAllocator::getInfo() {
    LOG_INFO("--------------------------------------");
    LOG_INFO("Used: {}, Free: {}", m_used, m_totalSize - m_used);
    printAllBlocks();
    LOG_INFO("--------------------------------------");
}

FreeListAllocator::~FreeListAllocator() noexcept {
    getInfo();
    std::free(m_memory);
}

void FreeListAllocator::combine(std::set<Block>::iterator newFreeBlockIt) {
    auto &newFreeBlock = (Block &)*newFreeBlockIt;
    auto lastFreeBlockIt = std::prev(newFreeBlockIt);
    auto nextFreeBlockIt = std::next(newFreeBlockIt);
    auto &lastFreeBlock = (Block &)*lastFreeBlockIt;
    bool deletedNewBlock = false;
    bool newBlockIsEnd = newFreeBlockIt == m_freeBlocks.end();
    bool newBlockIsBegin = newFreeBlockIt != m_freeBlocks.begin();
    if (!newBlockIsBegin) {
        if (lastFreeBlockIt->offset + lastFreeBlockIt->blockSize == newFreeBlockIt->offset) {
            lastFreeBlock.blockSize = lastFreeBlock.blockSize + newFreeBlockIt->blockSize;
            m_freeBlocks.erase(newFreeBlockIt);
            deletedNewBlock = true;
        } else {
            lastFreeBlock.next = &newFreeBlock;
        }
    }
    if (!newBlockIsEnd) {
        auto &nextFreeBlock = (Block &)*nextFreeBlockIt;
        if (deletedNewBlock &&
            lastFreeBlock.offset + lastFreeBlock.blockSize == nextFreeBlock.offset) {
            lastFreeBlock.blockSize += nextFreeBlock.blockSize;
            m_freeBlocks.erase(nextFreeBlockIt);
        } else if (newFreeBlock.offset + newFreeBlock.blockSize == nextFreeBlock.offset) {
            newFreeBlock.blockSize += nextFreeBlock.blockSize;
            m_freeBlocks.erase(nextFreeBlockIt);
        } else {
            newFreeBlock.next = &nextFreeBlock;
        }
    }
}

void FreeListAllocator::reset() {
    m_used = 0;
    const Block firstBlockFree(m_totalSize, nullptr, 0);
    m_freeBlocks.insert(firstBlockFree);
}

void FreeListAllocator::printAllBlocks() {
    for (const auto &item : m_occupiedBlocks) {
        LOG_INFO("Occupied Block size: {}, offset: {}", item.blockSize, item.offset);
    }
    for (const auto &item : m_freeBlocks) {
        LOG_INFO("Free Block size: {}, offset: {}", item.blockSize, item.offset);
    }
}
} // namespace Foundation