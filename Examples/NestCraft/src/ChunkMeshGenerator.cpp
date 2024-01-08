#include "ChunkMeshGenerator.hpp"

#include <iostream>

Mesh *ChunkMeshGenerator::generateMesh() {
    const uint32_t w = mChunk->getW(), h = mChunk->getH(), d = mChunk->getD();
    auto* vertices = new Vertex[w * h * d * 24];
    uint32_t verticesCount = 0;
    auto* indices = new uint32_t[w * h * d * 36];
    uint32_t indicesCount = 0;
    for (int y = 0; y < h; y++) {
        for (int z = 0; z < d; z++) {
            for (int x = 0; x < w; ++x) {
                if (isAir(x, y, z)) continue;

                float light;
                float uvSize = 1.f / 16.f;
//                float u = mChunk->get(x, y, )
                // Front
                if (isAir(x, y, z + 1)) {
                    light = 1.f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x-0.5f, y -0.5f, z + 0.5f, 0.f, 1.f, light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y -0.5f, z + 0.5f, 1.f, 1.f, light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y +0.5f, z + 0.5f, 1.f, 0.f, light);
                    vertices[verticesCount++] = Vertex(x-0.5f, y +0.5f, z + 0.5f, 0.f, 0.f, light);
                }
                // Back
                if (isAir(x, y, z - 1)) {
                    light = 0.5f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x-0.5f, y-0.5f, z-0.5f, 0.f, 1.f, light);
                    vertices[verticesCount++] = Vertex(x-0.5f, y+0.5f, z-0.5f, 1.f, 1.f, light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y+0.5f, z-0.5f, 1.f, 0.f, light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y-0.5f, z-0.5f, 0.f, 0.f, light);
                }
                // left
                if (isAir(x - 1, y, z)) {
                    light = 0.75;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y - 0.5f, z - 0.5f, 0.0f, 1.f, light);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y - 0.5f, z + 0.5f, 1.f, 1.f, light);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y + 0.5f, z + 0.5f, 1.f, 0.f, light);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y + 0.5f, z - 0.5f, 0.0f, 0.0f, light);
                }
                // right
                if (isAir(x + 1, y, z)) {
                    light = 1.f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y - 0.5f, z - 0.5f, 0.0f, 1.f, light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z - 0.5f, 1.f, 1.f, light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z + 0.5f, 1.f, 0.0f, light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y - 0.5f, z + 0.5f, 0.0f, 0.0f, light);
                }
                // top
                if (isAir(x, y + 1, z)) {
                    light = 1.f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y + 0.5f, z - 0.5f, 0.0f, 1.f, light);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y + 0.5f, z + 0.5f, 1.f, 1.f, light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z + 0.5f, 1.f, 0.0f, light);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z - 0.5f, 0.0f, 0.0f, light);
                }

                // Bottom
                if (isAir(x, y - 1, z)) {
                    light = 0.5f;
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x-0.5f, y-0.5f, z-0.5f, 0.f, 1.f, light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y-0.5f, z-0.5f, 1.f, 1.f, light);
                    vertices[verticesCount++] = Vertex(x+0.5f, y-0.5f, z+0.5f, 1.f, 0.f, light);
                    vertices[verticesCount++] = Vertex(x-0.5f, y-0.5f, z+0.5f, 0.f, 0.f, light);
                }
            }
        }
    }
    return new Mesh(vertices, verticesCount, indices,  indicesCount);
}

bool ChunkMeshGenerator::isAir(int x, int y, int z) {
    Voxel* voxel = mChunk->get(x, y, z);
    if (voxel == nullptr) {
        return true;
    }
    return mChunk->get(x, y, z)->isAir();
}

void ChunkMeshGenerator::addFaceIndices(uint32_t offset, uint32_t& indicesCount, uint32_t* indices) {
    indices[indicesCount++] = offset;
    indices[indicesCount++] = offset + 1;
    indices[indicesCount++] = offset + 2;
    indices[indicesCount++] = offset + 2;
    indices[indicesCount++] = offset + 3;
    indices[indicesCount++] = offset;
}