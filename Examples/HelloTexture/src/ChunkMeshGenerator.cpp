#include "ChunkMeshGenerator.hpp"

#include <iostream>

Mesh *ChunkMeshGenerator::generateMesh() {
    const uint32_t w = mChunk->getW(), h = mChunk->getH(), d = mChunk->getD();
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    for (int y = 0; y < h; y++) {
        for (int z = 0; z < d; z++) {
            for (int x = 0; x < w; ++x) {
                if (mChunk->get(x, y, z)->isAir()) continue;
                // left

                if (isAir(x - 1, y, z)) {
                    vertices.emplace_back(x - 0.5f, y - 0.5f, z - 0.5f, 0.0f, 0.f);
                    vertices.emplace_back(x - 0.5f, y - 0.5f, z + 0.5f, 1.f, 0.f);
                    vertices.emplace_back(x - 0.5f, y + 0.5f, z + 0.5f, 1.f, 1.f);
                    vertices.emplace_back(x - 0.5f, y + 0.5f, z - 0.5f, 0.0f, 1.0f);

//                    vertices.emplace_back(x+0.5f, y-0.5f, z-0.5f, 0.0f, 1.f),
//                    vertices.emplace_back(x+0.5f, y+0.5f, z-0.5f, 1.f, 1.f),
//                    vertices.emplace_back(x+0.5f, y+0.5f, z+0.5f, 1.f, 0.0f),
//                    vertices.emplace_back(x+0.5f, y-0.5f, z+0.5f, 0.0f, 0.0f);
                    std::cout << indices.size() << std::endl;
                    addFaceIndices(indices.size(), indices);
                    std::cout << "Point 1:" << " Y: " << y - 0.5f << " Z: " << z - 0.5f << std::endl;
                    std::cout << "Point 2:" << " Y: " << y - 0.5f << " Z: " << z + 0.5f << std::endl;
                    std::cout << "Point 3:" << " Y: " << y + 0.5f << " Z: " << z + 0.5f << std::endl;
                    std::cout << "Point 4:" << " Y: " << y + 0.5f << " Z: " << z - 0.5f << std::endl << std::endl;
                }

            }
        }
    }
    return new Mesh(&vertices[0], vertices.size(), &indices[0],  indices.size());
}

bool ChunkMeshGenerator::isAir(int x, int y, int z) {
    Voxel* voxel = mChunk->get(x, y, z);
    if (voxel == nullptr) {
        return true;
    }
    return mChunk->get(x, y, z)->isAir();
}

void ChunkMeshGenerator::addFaceIndices(uint32_t offset, std::vector<uint32_t> &indices) {
    indices.emplace_back(offset);
    indices.emplace_back(offset + 1);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 2);
    indices.emplace_back(offset + 3);
    indices.emplace_back(offset);
}