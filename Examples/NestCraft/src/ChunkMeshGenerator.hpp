#pragma once

#include <Nest.hpp>

#include "Chunk.hpp"

class ChunkMeshGenerator {
public:
    Mesh* generateMesh(Chunk* chunk, int chunkIndexX, int chunkIndexY, int chunkIndexZ);
private:
    bool isAir(int x, int y, int z, Chunk* chunk);
    void addFaceIndices(uint32_t offset, uint32_t& indicesCount, uint32_t* indices);
    float ambientOcclusionFactor = 0.2f;
};