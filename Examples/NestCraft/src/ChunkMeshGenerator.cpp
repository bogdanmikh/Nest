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

Mesh* ChunkMeshGenerator::generateMesh(Chunk *chunk) {
    auto* vertices = new Vertex[ChunkManager::SIZE_X * ChunkManager::SIZE_Y * ChunkManager::SIZE_Y * 24];
    uint32_t verticesCount = 0;
    auto* indices = new uint32_t[ChunkManager::SIZE_X * ChunkManager::SIZE_Y * ChunkManager::SIZE_Y * 36];
    uint32_t indicesCount = 0;
    for (int voxelIndexY = 0; voxelIndexY < ChunkManager::SIZE_Y; voxelIndexY++) {
        for (int voxelIndexZ = 0; voxelIndexZ < ChunkManager::SIZE_Y; voxelIndexZ++) {
            for (int voxelIndexX = 0; voxelIndexX < ChunkManager::SIZE_X; ++voxelIndexX) {
                int x = voxelIndexX + chunk->getPosX() * ChunkManager::SIZE_X;
                int y = voxelIndexY + chunk->getPosY() * ChunkManager::SIZE_Y;
                int z = voxelIndexZ + chunk->getPosZ() * ChunkManager::SIZE_Z;
//                std::cout << std::endl << x << " " << y << " " << z;
                if (isAir(voxelIndexX, voxelIndexY, voxelIndexZ, chunk)) continue;

                uint8_t voxelId = chunk->getType(voxelIndexX, voxelIndexY, voxelIndexZ);
                float light;
                float uvSize = 1.f / 16.f;
                uvSize -= 0.001;
                glm::vec2 uv = getUV(voxelId);
                // Front
                if (isAir(voxelIndexX, voxelIndexY, voxelIndexZ + 1, chunk)) {
                    light = 1.0f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x-0.5f, y-0.5f, z+0.5f,
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
                if (isAir(voxelIndexX, voxelIndexY, voxelIndexZ - 1, chunk)) {
                    light = 0.75f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x-0.5f, y-0.5f, z-0.5f,
                                                       uv.x+uvSize,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x-0.5f, y+0.5f, z-0.5f,
                                                       uv.x+uvSize,
                                                       uv.y,
                                                       light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y+0.5f, z-0.5f,
                                                       uv.x,
                                                       uv.y,
                                                       light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y-0.5f, z-0.5f,
                                                       uv.x,
                                                       uv.y+uvSize,
                                                       light);
                }
                // top
                if (isAir(voxelIndexX, voxelIndexY + 1, voxelIndexZ, chunk)) {
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
                if (isAir(voxelIndexX, voxelIndexY - 1, voxelIndexZ, chunk)) {
                    light = 0.55f;
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
                                                       uv.x,
                                                       uv.y,
                                                       light);
                }

                // right
                if (isAir(voxelIndexX + 1, voxelIndexY, voxelIndexZ, chunk)) {
                    light = 0.9f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y - 0.5f, z - 0.5f,
                                                       uv.x+uvSize,
                                                       uv.y,
                                                       light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z - 0.5f,
                                                       uv.x+uvSize,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z + 0.5f,
                                                       uv.x,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y - 0.5f, z + 0.5f,
                                                       uv.x,
                                                       uv.y,
                                                       light);
                }
                // left
                if (isAir(voxelIndexX - 1, voxelIndexY, voxelIndexZ, chunk)) {
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
            }
        }
    }
    return new Mesh(vertices, verticesCount, indices,  indicesCount);
}

Mesh *ChunkMeshGenerator::generateMesh(ChunkManager *chunkManager, int chunkIndexX, int chunkIndexY, int chunkIndexZ) {
    Chunk *chunk = chunkManager->getChunk(chunkIndexX, chunkIndexY, chunkIndexZ);

    auto* vertices = new Vertex[ChunkManager::SIZE_X * ChunkManager::SIZE_Y * ChunkManager::SIZE_Y * 24];
    uint32_t verticesCount = 0;
    auto* indices = new uint32_t[ChunkManager::SIZE_X * ChunkManager::SIZE_Y * ChunkManager::SIZE_Y * 36];
    uint32_t indicesCount = 0;
    for (int voxelIndexY = 0; voxelIndexY < ChunkManager::SIZE_Y; voxelIndexY++) {
        for (int voxelIndexZ = 0; voxelIndexZ < ChunkManager::SIZE_Y; voxelIndexZ++) {
            for (int voxelIndexX = 0; voxelIndexX < ChunkManager::SIZE_X; ++voxelIndexX) {
                int x = voxelIndexX + chunk->getPosX() * ChunkManager::SIZE_X;
                int y = voxelIndexY + chunk->getPosY() * ChunkManager::SIZE_Y;
                int z = voxelIndexZ + chunk->getPosZ() * ChunkManager::SIZE_Z;
//                std::cout << std::endl << x << " " << y << " " << z;
                if (isAir(voxelIndexX, voxelIndexY, voxelIndexZ, chunk)) continue;

                uint8_t voxelId = chunk->getType(voxelIndexX, voxelIndexY, voxelIndexZ);
                float light;
                float uvSize = 1.f / 16.f;
                uvSize -= 0.001;
                glm::vec2 uv = getUV(voxelId);
                // Front
                if (isAir(voxelIndexX, voxelIndexY, voxelIndexZ + 1, chunk)) {
                    light = 1.0f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x-0.5f, y-0.5f, z+0.5f,
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
                if (isAir(voxelIndexX, voxelIndexY, voxelIndexZ - 1, chunk)) {
                    light = 0.75f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x-0.5f, y-0.5f, z-0.5f,
                                                       uv.x+uvSize,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x-0.5f, y+0.5f, z-0.5f,
                                                       uv.x+uvSize,
                                                       uv.y,
                                                       light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y+0.5f, z-0.5f,
                                                       uv.x,
                                                       uv.y,
                                                       light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y-0.5f, z-0.5f,
                                                       uv.x,
                                                       uv.y+uvSize,
                                                       light);
                }
                // top
                if (isAir(voxelIndexX, voxelIndexY + 1, voxelIndexZ, chunk)) {
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
                if (isAir(voxelIndexX, voxelIndexY - 1, voxelIndexZ, chunk)) {
                    light = 0.55f;
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
                                                       uv.x,
                                                       uv.y,
                                                       light);
                }

                // right
                if (isAir(voxelIndexX + 1, voxelIndexY, voxelIndexZ, chunk)) {
                    light = 0.9f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y - 0.5f, z - 0.5f,
                                                       uv.x+uvSize,
                                                       uv.y,
                                                       light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z - 0.5f,
                                                       uv.x+uvSize,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z + 0.5f,
                                                       uv.x,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y - 0.5f, z + 0.5f,
                                                       uv.x,
                                                       uv.y,
                                                       light);
                }
                // left
                if (isAir(voxelIndexX - 1, voxelIndexY, voxelIndexZ, chunk)) {
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
            }
        }
    }
    return new Mesh(vertices, verticesCount, indices,  indicesCount);
}

bool ChunkMeshGenerator::isAir(int x, int y, int z, Chunk* chunk) {
    Voxel* voxel = chunk->get(x, y, z);
    if (voxel == nullptr) {
        return false;
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
