#pragma once

#include <Nest.hpp>
#include "ChunksStorage.hpp"

class ChunkMeshGenerator {
public:
    Mesh* generateMesh(ChunksStorage* chunkManager, int chunkIndexX, int chunkIndexY, int chunkIndexZ);
private:
    bool isAir(int localX, int localY, int localZ, ChunksStorage *chunkManager);
    void addFaceIndices(uint32_t offset, uint32_t& indicesCount, uint32_t* indices);
};