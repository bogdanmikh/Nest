#pragma once

#include "Chunk.hpp"

class ChunkManager {
public:
    ChunkManager(uint32_t w, uint32_t h, uint32_t d);
    ~ChunkManager();
    Chunk** chunks;
    uint32_t getSize();
    uint32_t getSizeX() const;
    uint32_t getSizeY() const;
    uint32_t getSizeZ() const;
private:
    uint32_t size;
    uint32_t w, h, d;
};
