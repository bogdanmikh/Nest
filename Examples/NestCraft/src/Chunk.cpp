#include "Chunk.hpp"
#include "ChunksStorage.hpp"

#include <cmath>

Chunk::Chunk() {
    data = new Voxel[ChunksStorage::SIZE_X * ChunksStorage::SIZE_Y * ChunksStorage::SIZE_Z];
    for (int x = 0; x < ChunksStorage::SIZE_X; ++x) {
        for (int y = 0; y < ChunksStorage::SIZE_Y; y++) {
            for (int z = 0; z < ChunksStorage::SIZE_Z; ++z) {
                set(x, y, z, VoxelType::GROUND);
            }
        }
    }
}

Chunk::~Chunk() {
    delete[] data;
}

void Chunk::set(int32_t indexX, int32_t indexY, int32_t indexZ, VoxelType type) {
    if (indexX < 0 || indexY < 0 || indexZ < 0 || indexX >= ChunksStorage::SIZE_X || indexY >= ChunksStorage::SIZE_Y || indexZ >= ChunksStorage::SIZE_Z) {
        return;
    }
    data[indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z + indexX * ChunksStorage::SIZE_X + indexZ].type = type;
}

Voxel* Chunk::get(int32_t indexX, int32_t indexY, int32_t indexZ) {
    if (indexX < 0 || indexY < 0 || indexZ < 0 || indexX >= ChunksStorage::SIZE_X || indexY >= ChunksStorage::SIZE_Y || indexZ >= ChunksStorage::SIZE_Z) {
        return nullptr;
    }
    return &data[indexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z + indexX * ChunksStorage::SIZE_X + indexZ];
}

uint8_t Chunk::getType(int32_t indexX, int32_t indexY, int32_t indexZ) {
    Voxel* voxel = get(indexX, indexY, indexZ);
    if (voxel == nullptr) {
        return 0;
    }
    return uint8_t(voxel->type);
}
