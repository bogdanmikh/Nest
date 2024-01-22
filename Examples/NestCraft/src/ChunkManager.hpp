#pragma once

#include <cstdint>
#include "Chunk.hpp"

class ChunkManager {
public:
    static const uint32_t SIZE_X  = 10;
    static const uint32_t SIZE_Y = 10;
    static const uint32_t SIZE_Z  = 10;
    ChunkManager(uint32_t w, uint32_t h, uint32_t d);
    ~ChunkManager();
    uint32_t getSize() const;
    uint32_t getSizeX() const;
    uint32_t getSizeY() const;
    uint32_t getSizeZ() const;
    Chunk* getChunk(int chunkIndexX, int chunkIndexY, int chunkIndexZ);
    Chunk* getChunkIndex(int index);
private:
    Chunk** chunks;
    uint32_t size;
    uint32_t w, h, d;
};
