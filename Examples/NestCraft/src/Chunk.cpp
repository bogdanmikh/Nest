#include "Chunk.hpp"
#include <cmath>

Chunk::Chunk(uint32_t wight, uint32_t height, uint32_t depth)
  : mWight(wight), mHeight(height), mDepth(depth), mSize(mWight * mHeight * mDepth) {
    chunk = new Voxel[mSize];
    for (int y = 0; y < mHeight; y++) {
        for (int z = 0; z < mDepth; ++z) {
            for (int x = 0; x < mWight; ++x) {
//                if (y <= ((sin(x * 0.6f) * 0.5 + 0.5f) * 5)) {
//                    set(x, y, z, VoxelType::GROUND);
//                } else if (y <= ((sin(x * 0.6f) * 0.5 + 0.5f) * 6)) {
//                    set(x, y, z, VoxelType::GRASS);
//                }
                if (y < 100) set(x, y, z, VoxelType::GROUND);
            }
        }
    }
}

Chunk::~Chunk() {
    delete chunk;
}
