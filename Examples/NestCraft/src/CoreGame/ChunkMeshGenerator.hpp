#pragma once

#include "Nest.hpp"
#include "ChunksStorage.hpp"

class ChunkMeshGenerator {
public:
    Mesh* generateMesh(ChunksStorage* chunksStorage, int chunkIndexX, int chunkIndexY, int chunkIndexZ, bool ambientOcclusion);
private:
    bool isAir(int localX, int localY, int localZ, ChunksStorage *chunkManager);
    void addFaceIndices(uint32_t offset, uint32_t& indicesCount, uint32_t* indices);
    float ambientOcclusionFactor = 0.25f;
};