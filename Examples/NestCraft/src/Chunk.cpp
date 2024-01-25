#include "Chunk.hpp"
#include "ChunksStorage.hpp"

#include <cmath>

Chunk::Chunk() {
    needUpdate = false;
    data = new Voxel[SIZE_X * SIZE_Y * SIZE_Z];
    for (int x = 0; x < SIZE_X; ++x) {
        for (int y = 0; y < SIZE_Y; y++) {
            for (int z = 0; z < SIZE_Z; ++z) {
                set(x, y, z, VoxelType::GROUND);
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
    data[y * Chunk::SIZE_X * Chunk::SIZE_Z + x * Chunk::SIZE_X + z].type = type;
}

Voxel *Chunk::get(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0 || x >= SIZE_X || y >= SIZE_Y || z >= SIZE_Z) {
        return nullptr;
    }
    return &data[y * Chunk::SIZE_X * Chunk::SIZE_Z + x * Chunk::SIZE_X + z];
}


uint8_t Chunk::getType(int indexX, int indexY, int indexZ) {
    Voxel* voxel = get(indexX, indexY, indexZ);
    if (voxel == nullptr) {
        return 0;
    }
    return uint8_t(voxel->type);
}
