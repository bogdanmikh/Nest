#include "ChunkMeshGenerator.hpp"

#include <iostream>

Mesh *ChunkMeshGenerator::generateMesh() {
    const uint32_t w = mChunk->getW(), h = mChunk->getH(), d = mChunk->getD();
    auto* vertices = new Vertex[w * h * d * 24];
    uint32_t verticesCount = 0;
    auto* indices = new uint32_t[w * h * d * 6];
    uint32_t indicesCount = 0;
    for (int y = 0; y < h; y++) {
        for (int z = 0; z < d; z++) {
            for (int x = 0; x < w; ++x) {
                if (isAir(x, y, z)) continue;
                // left
                if (isAir(x - 1, y, z)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y - 0.5f, z - 0.5f, 0.0f, 1.f);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y - 0.5f, z + 0.5f, 1.f, 1.f);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y + 0.5f, z + 0.5f, 1.f, 0.f);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y + 0.5f, z - 0.5f, 0.0f, 0.0f);
                }
                // right
                if (isAir(x + 1, y, z)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y - 0.5f, z - 0.5f, 0.0f, 1.f);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z - 0.5f, 1.f, 1.f);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z + 0.5f, 1.f, 0.0f);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y - 0.5f, z + 0.5f, 0.0f, 0.0f);
                }
                // top
                if (isAir(x, y + 1, z)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y + 0.5f, z - 0.5f, 0.0f, 1.f);
                    vertices[verticesCount++] = Vertex(x - 0.5f, y + 0.5f, z + 0.5f, 1.f, 1.f);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z + 0.5f, 1.f, 0.0f);
                    vertices[verticesCount++] = Vertex(x + 0.5f, y + 0.5f, z - 0.5f, 0.0f, 0.0f);
                }

                // Bottom
                if (isAir(x, y - 1, z)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x-0.5f, y-0.5f, z-0.5f, 0.f, 1.f);
                    vertices[verticesCount++] = Vertex(x+0.5f, y-0.5f, z-0.5f, 1.f, 1.f);
                    vertices[verticesCount++] = Vertex(x+0.5f, y-0.5f, z+0.5f, 1.f, 0.f);
                    vertices[verticesCount++] = Vertex(x-0.5f, y-0.5f, z+0.5f, 0.f, 0.f);
                }
                // Front
                if (isAir(x, y, z + 1)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x-0.5f, y -0.5f, z + 0.5f, 0.f, 1.f);
                    vertices[verticesCount++] = Vertex(x+0.5f, y -0.5f, z + 0.5f, 1.f, 1.f);
                    vertices[verticesCount++] = Vertex(x+0.5f, y +0.5f, z + 0.5f, 1.f, 0.f);
                    vertices[verticesCount++] = Vertex(x-0.5f, y +0.5f, z + 0.5f, 0.f, 0.f);
                }
                // Back
                if (isAir(x, y, z - 1)) {
                    addFaceIndices(verticesCount, indicesCount, indices);
                    vertices[verticesCount++] = Vertex(x-0.5f, y-0.5f, z-0.5f, 0.f, 1.f);
                    vertices[verticesCount++] = Vertex(x-0.5f, y+0.5f, z-0.5f, 1.f, 1.f);
                    vertices[verticesCount++] = Vertex(x+0.5f, y+0.5f, z-0.5f, 1.f, 0.f);
                    vertices[verticesCount++] = Vertex(x+0.5f, y-0.5f, z-0.5f, 0.f, 0.f);
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