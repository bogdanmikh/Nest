#include "ChunkMeshGenerator.hpp"

#include <iostream>

Mesh *ChunkMeshGenerator::generateMesh() {
    const uint32_t w = mChunk->getW(), h = mChunk->getH(), d = mChunk->getD();
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    for (int y = 0; y < h; y++) {
        for (int z = 0; z < d; ++z) {
            for (int x = 0; x < w; ++x) {
                auto id = mChunk->get(x, y, z);
                if (!id) continue;

                // left
                if (isAir(x - 1, y, z)) {
                    vertices.emplace_back(x-0.5f, y-0.5f, z-0.5f, 0.0f, 1.f);
                    vertices.emplace_back(x-0.5f, y-0.5f, z+0.5f, 1.f, 1.f);
                    vertices.emplace_back(x-0.5f, y+0.5f, z+0.5f, 1.f, 0.f);
                    vertices.emplace_back(x-0.5f, y+0.5f, z-0.5f, 0.0f, 0.0f);

                    int offset = indices.size();
                    indices.push_back(offset);
                    indices.push_back(offset + 1);
                    indices.push_back(offset + 2);
                    indices.push_back(offset + 2);
                    indices.push_back(offset + 3);
                    indices.push_back(offset);
                    std::cout << "X: " << x << " Y: " << y << " Z: " << z << std::endl;
                }

            }
        }
    }
    return new Mesh(&vertices[0], vertices.size(), &indices[0],  indices.size());
}

bool ChunkMeshGenerator::isAir(int x, int y, int z) {
    const uint32_t w = mChunk->getW(), h = mChunk->getH(), d = mChunk->getD();
    if (!(x >= 0 && y >= 0 && z >= 0 && x < w && y < h && z < d)) {
        return true;
    }
    return mChunk->get(x, y, z) == 0;
}
