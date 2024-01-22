#include "Chunk.hpp"
#include "ChunkManager.hpp"

#include <cmath>

Chunk::Chunk(int posX, int posY, int posZ) : posX(posX), posY(posY), posZ(posZ)  {
    chunk = new Voxel[ChunkManager::SIZE_X * ChunkManager::SIZE_Y * ChunkManager::SIZE_Z];
    for (int y = 0; y < ChunkManager::SIZE_Y; y++) {
        for (int z = 0; z < ChunkManager::SIZE_Z; ++z) {
            for (int x = 0; x < ChunkManager::SIZE_X; ++x) {
                if (y <= ((sin(x * 0.6f) * 0.5 + 0.5f) * 5)) {
                    set(x, y, z, VoxelType::GROUND);
                } else if (y <= ((sin(x * 0.6f) * 0.5 + 0.5f) * 6)) {
                    set(x, y, z, VoxelType::GRASS);
                }
//                set(x, y, z, VoxelType::GROUND);
            }
        }
    }
}

Chunk::~Chunk() {
    delete chunk;
}

void Chunk::set(uint32_t x, uint32_t y, uint32_t z, VoxelType type) {
    chunk[y * ChunkManager::SIZE_X * ChunkManager::SIZE_Y + x * ChunkManager::SIZE_X + z].type = type;
}

Voxel* Chunk::get(int32_t x, int32_t y, int32_t z) {
    if (x < 0 || y < 0 || z < 0 || x >= ChunkManager::SIZE_X || y >= ChunkManager::SIZE_Y || z >= ChunkManager::SIZE_Z) {
        return nullptr;
    }
    return &chunk[y * ChunkManager::SIZE_X * ChunkManager::SIZE_Y + x * ChunkManager::SIZE_X + z];
}

uint8_t Chunk::getType(int32_t x, int32_t y, int32_t z) {
    Voxel* voxel = get(x, y, z);
    if (voxel == nullptr) {
        return 0;
    }
    return uint8_t(voxel->type);
}

int Chunk::getPosX() const {
    return posX;
}

int Chunk::getPosY() const {
    return posY;
}

int Chunk::getPosZ() const {
    return posZ;
}
