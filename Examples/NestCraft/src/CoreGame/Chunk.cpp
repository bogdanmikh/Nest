#include "Chunk.hpp"
#include "ChunksStorage.hpp"

#include <random>
#include "glm/gtc/noise.hpp"
#include <cmath>

Chunk::Chunk() {
    data = new Voxel[SIZE_X * SIZE_Y * SIZE_Z];
    for (int x = 0; x < SIZE_X; ++x) {
        for (int z = 0; z < SIZE_Z; ++z) {
            int realX = x + SIZE_X;
            int realZ = z + SIZE_Z;
            float height = glm::perlin(glm::vec3(realX * 0.05f, realZ * 0.05f, 0));
            for (int y = 0; y < SIZE_Y; y++) {
                if (y <= height * 5 + 10) set(x, y, z, VoxelType(12));
            }
        }
    }
}

Chunk::~Chunk() {
    delete[] data;
}

void Chunk::set(int x, int y, int z, VoxelType type) {
    if (x < 0 || y < 0 || z < 0 || x >= SIZE_X || y >= SIZE_Y || z >= SIZE_Z)
        return;
    data[y * Chunk::SIZE_X * Chunk::SIZE_Z + x * Chunk::SIZE_Z + z].type = type;
}

Voxel *Chunk::get(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0 || x >= SIZE_X || y >= SIZE_Y || z >= SIZE_Z) {
        return nullptr;
    }
    return &data[y * Chunk::SIZE_X * Chunk::SIZE_Z + x * Chunk::SIZE_Z + z];
}


uint8_t Chunk::getType(int indexX, int indexY, int indexZ) {
    Voxel* voxel = get(indexX, indexY, indexZ);
    if (voxel == nullptr) {
        return 0;
    }
    return uint8_t(voxel->type);
}

void Chunk::setMesh(Mesh* mesh) {
    m_mesh = mesh;
}

Mesh* Chunk::getMesh() {
    return m_mesh;
}
