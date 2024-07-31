#pragma once

#include "Nest.hpp"
#include "ChunksStorage.hpp"

class ChunkMeshGenerator {
public:
    static Mesh *generateMesh(
        ChunksStorage *chunksStorage,
        int chunkIndexX,
        int chunkIndexY,
        int chunkIndexZ,
        bool ambientOcclusion
    );

private:
    static bool isAir(int localX, int localY, int localZ, ChunksStorage *chunkManager);
    static void addFaceIndices(uint32_t offset, uint32_t &indicesCount, uint32_t *indices);
    static constexpr float ambientOcclusionFactor = 0.2f;
};