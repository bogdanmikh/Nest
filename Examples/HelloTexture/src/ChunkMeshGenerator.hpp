#pragma once

#include <Nest.hpp>

#include "Chunk.hpp"

class ChunkMeshGenerator {
public:
    explicit ChunkMeshGenerator(Chunk* chunk) : mChunk(chunk) {}
    inline void updateChunk(Chunk* chunk) {
        mChunk = chunk;
    }
    Mesh* generateMesh();
private:
    bool isAir(int x, int y, int z);
    Chunk *mChunk;
};