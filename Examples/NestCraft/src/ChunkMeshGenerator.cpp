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

Mesh* ChunkMeshGenerator::generateMesh(ChunksStorage *chunksStorage, int chunkIndexX, int chunkIndexY, int chunkIndexZ) {
    Chunk* currentChunk = chunksStorage->getChunk(chunkIndexX, chunkIndexY, chunkIndexZ);
    auto* vertices = new Vertex[Chunk::SIZE_X * Chunk::SIZE_Y * Chunk::SIZE_Z * 24];
    auto* indices = new uint32_t[Chunk::SIZE_X * Chunk::SIZE_Y * Chunk::SIZE_Z * 36];
    uint32_t verticesCount = 0;
    uint32_t indicesCount = 0;
    for (int voxelIndexX = 0; voxelIndexX < Chunk::SIZE_X; voxelIndexX++) {
        for (int voxelIndexY = 0; voxelIndexY < Chunk::SIZE_Y; voxelIndexY++) {
            for (int voxelIndexZ = 0; voxelIndexZ < Chunk::SIZE_Z; voxelIndexZ++) {
                int x = voxelIndexX + chunkIndexX * Chunk::SIZE_X;
                int y = voxelIndexY + chunkIndexY * Chunk::SIZE_Y;
                int z = voxelIndexZ + chunkIndexZ * Chunk::SIZE_Z;
//                std::cout << std::endl << x << " " << y << " " << z << std::endl;

                uint8_t voxelId = currentChunk->getType(voxelIndexX, voxelIndexY, voxelIndexZ);

                if (isAir(x, y, z, chunksStorage)) continue;

                float light;
                float uvSize = 1.f / 16.f;
                uvSize -= 0.001;
                glm::vec2 uv = getUV(voxelId);
                // Front
                if (isAir(x, y, z + 1, chunksStorage)) {
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
                if (isAir(x, y, z - 1, chunksStorage)) {
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
                // Top
                if (isAir(x, y + 1, z, chunksStorage)) {
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
                if (isAir(x, y - 1, z, chunksStorage)) {
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

                // Right
                if (isAir(x + 1, y, z, chunksStorage)) {
                    light = 0.9f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z - 0.5f,
                                                       uv.x+uvSize,
                                                       uv.y,
                                                       light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y - 0.5f, z - 0.5f,
                                                       uv.x+uvSize,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y - 0.5f, z + 0.5f,
                                                       uv.x,
                                                       uv.y+uvSize,
                                                       light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z + 0.5f,
                                                       uv.x,
                                                       uv.y,
                                                       light);
                }
                // left
                if (isAir(x - 1, y, z, chunksStorage)) {
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

bool ChunkMeshGenerator::isAir(int localX, int localY, int localZ, ChunksStorage *chunksStorage) {
    Voxel *voxel = chunksStorage->getVoxel(localX, localY, localZ);
    if (voxel == nullptr) {
        return true;
    }
    return voxel->isAir();
}

void ChunkMeshGenerator::addFaceIndices(uint32_t offset, uint32_t& indicesCount, uint32_t* indices) {
    indices[indicesCount++] = offset;
    indices[indicesCount++] = offset + 1;
    indices[indicesCount++] = offset + 2;
    indices[indicesCount++] = offset + 2;
    indices[indicesCount++] = offset + 3;
    indices[indicesCount++] = offset;
}
