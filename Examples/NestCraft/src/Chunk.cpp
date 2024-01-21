#include "Chunk.hpp"
#include <cmath>

Chunk::Chunk(int posX, int posY, int posZ) : posX(posX), posY(posY), posZ(posZ)  {
    chunk = new Voxel[wight * height * depth];
    for (int y = 0; y < height; y++) {
        for (int z = 0; z < depth; ++z) {
            for (int x = 0; x < wight; ++x) {
//                if (y <= ((sin(x * 0.6f) * 0.5 + 0.5f) * 5)) {
//                    set(x, y, z, VoxelType::GROUND);
//                } else if (y <= ((sin(x * 0.6f) * 0.5 + 0.5f) * 6)) {
//                    set(x, y, z, VoxelType::GRASS);
//                }
                set(x, y, z, VoxelType::GROUND);
            }
        }
    }
}

Chunk::~Chunk() {
    delete chunk;
}

void Chunk::set(uint32_t x, uint32_t y, uint32_t z, VoxelType type) {
    chunk[y * wight * height + x * wight + z].type = type;
}

Voxel* Chunk::get(int32_t x, int32_t y, int32_t z) {
    if (x < 0 || y < 0 || z < 0 || x >= wight || y >= height || z >= depth) {
        return nullptr;
    }
    return &chunk[y * wight * height + x * wight + z];
}

uint8_t Chunk::getType(int32_t x, int32_t y, int32_t z) {
    if (x < 0 || y < 0 || z < 0 || x >= wight || y >= height || z >= depth) {
        return 0;
    }
    return uint8_t(chunk[y * wight * height + x * wight + z].type);
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
