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
    void addFaceIndices(uint32_t offset, std::vector<uint32_t> &indices);
    Chunk *mChunk;
};