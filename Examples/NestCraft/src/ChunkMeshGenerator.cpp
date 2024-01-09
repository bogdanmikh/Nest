#include "ChunkMeshGenerator.hpp"

#include <iostream>

glm::vec2 getUV(uint8_t tileIndex) {
    float uvSize = 1.0f / 16.f;
    float u = tileIndex % 16 * uvSize;
    float v = tileIndex / 16 * uvSize;
    u += 0.0005f;
    v += 0.0005f;
    return {u, v};
}

Mesh *ChunkMeshGenerator::generateMesh(Chunk *chunk, int chunkIndexX, int chunkIndexY, int chunkIndexZ) {
    const uint32_t w = chunk->getW(), h = chunk->getH(), d = chunk->getD();
    auto* vertices = new Vertex[w * h * d * 24];
    uint32_t verticesCount = 0;
    auto* indices = new uint32_t[w * h * d * 36];
    uint32_t indicesCount = 0;
    for (int voxelIndexY = 0; voxelIndexY < h; voxelIndexY++) {
        for (int voxelIndexZ= 0; voxelIndexZ < d; voxelIndexZ++) {
            for (int voxelIndexX = 0; voxelIndexX < w; ++voxelIndexX) {
                int x = voxelIndexX + chunkIndexX * w;
                int y = voxelIndexY + chunkIndexY * y;
                int z = voxelIndexZ + chunkIndexZ * z;
                if (isAir(x, y, z, chunk)) continue;

                uint8_t voxelId = chunk->getType(x, y, z);
                float light;
                float uvSize = 1.0f / 16.f;
                glm::vec2 uv = getUV(voxelId);

                // Front
                if (isAir(x, y, z + 1, chunk)) {
                    light = 1.0f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x-0.5f, y -0.5f, z + 0.5f,
                                                       uv.x,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y -0.5f, z + 0.5f,
                                                       uv.x+uvSize,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y +0.5f, z + 0.5f,
                                                       uv.x+uvSize,
                                                       uv.y,
                                                       light);
                    vertices[verticesCount++] = Vertex(x-0.5f, y +0.5f, z + 0.5f,
                                                       uv.x,
                                                       uv.y,
                                                       light);
                }
                // Back
                if (isAir(x, y, z - 1, chunk)) {
                    light = 0.75f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x-0.5f, y-0.5f, z-0.5f,
                                                       uv.x,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x-0.5f, y+0.5f, z-0.5f,
                                                       uv.x+uvSize,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y+0.5f, z-0.5f,
                                                       uv.x+uvSize,
                                                       uv.y,
                                                       light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y-0.5f, z-0.5f,
                                                       uv.x,
                                                       uv.y,
                                                       light);
                }
                // top
                if (isAir(x, y + 1, z, chunk)) {
                    light = 0.95f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y + 0.5f, z - 0.5f,
                                                       uv.x,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y + 0.5f, z + 0.5f,
                                                       uv.x+uvSize,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z + 0.5f,
                                                       uv.x+uvSize,
                                                       uv.y,
                                                       light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z - 0.5f,
                                                       uv.x,
                                                       uv.y,
                                                       light);
                }

                // Bottom
                if (isAir(x, y - 1, z, chunk)) {
                    light = 0.85f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x-0.5f, y-0.5f, z-0.5f,
                                                       uv.x,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y-0.5f, z-0.5f,
                                                       uv.x+uvSize,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y-0.5f, z+0.5f,
                                                       uv.x+uvSize,
                                                       uv.y,
                                                       light);
                    vertices[verticesCount++] = Vertex(x-0.5f, y-0.5f, z+0.5f,
                                                       uv.y,
                                                       uv.y,
                                                       light);
                }

                // left
                if (isAir(x - 1, y, z, chunk)) {
                    light = 0.8f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y - 0.5f, z - 0.5f,
                                                       uv.x,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y - 0.5f, z + 0.5f,
                                                       uv.x+uvSize,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y + 0.5f, z + 0.5f,
                                                       uv.x+uvSize,
                                                       uv.y,
                                                       light);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y + 0.5f, z - 0.5f,
                                                       uv.x,
                                                       uv.y,
                                                       light);
                }
                // right
                if (isAir(x + 1, y, z, chunk)) {
                    light = 0.9f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y - 0.5f, z - 0.5f,
                                                       uv.x,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z - 0.5f,
                                                       uv.x+uvSize,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z + 0.5f,
                                                       uv.x+uvSize,
                                                       uv.y,
                                                       light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y - 0.5f, z + 0.5f,
                                                       uv.x,
                                                       uv.y,
                                                       light);
                }
            }
        }
    }
    return new Mesh(vertices, verticesCount, indices,  indicesCount);
}

bool ChunkMeshGenerator::isAir(int x, int y, int z, Chunk* chunk) {
    Voxel* voxel = chunk->get(x, y, z);
    if (voxel == nullptr) {
        return true;
    }
    return chunk->get(x, y, z)->isAir();
}

void ChunkMeshGenerator::addFaceIndices(uint32_t offset, uint32_t& indicesCount, uint32_t* indices) {
    indices[indicesCount++] = offset;
    indices[indicesCount++] = offset + 1;
    indices[indicesCount++] = offset + 2;
    indices[indicesCount++] = offset + 2;
    indices[indicesCount++] = offset + 3;
    indices[indicesCount++] = offset;
}
