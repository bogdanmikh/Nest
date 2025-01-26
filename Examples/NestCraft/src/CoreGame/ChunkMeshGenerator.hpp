#pragma once

#include "ChunksStorage.hpp"
#include "Nest.hpp"

namespace ChunkMeshGenerator {

struct Vertex {
    // position
    glm::vec3 Position;
    // texCoords
    glm::vec2 TexCoords;
    // light
    float light;

    Vertex()
        : Position(0, 0, 0)
        , TexCoords(0, 0)
        , light(0) {};

    Vertex(glm::vec3 aPosition, glm::vec2 aTexCoords, float light = 1.f)
        : Position(aPosition)
        , TexCoords(aTexCoords)
        , light(light) {};

    Vertex(float x, float y, float z, float aTexCoordX, float aTexCoordY, float light = 1.f)
        : Position(x, y, z)
        , TexCoords(aTexCoordX, aTexCoordY)
        , light(light) {};
};

Nest::StaticMesh *generateMesh(
    Bird::TextureHandle textureHandle,
    Bird::ProgramHandle programHandle,
    ChunksStorage *chunksStorage,
    int chunkIndexX,
    int chunkIndexY,
    int chunkIndexZ,
    bool ambientOcclusion
);

bool isAir(int localX, int localY, int localZ, ChunksStorage *chunkManager);
void addFaceIndices(uint32_t offset, uint32_t &indicesCount, uint32_t *indices);
constexpr float ambientOcclusionFactor = 0.2f;
}; // namespace ChunkMeshGenerator