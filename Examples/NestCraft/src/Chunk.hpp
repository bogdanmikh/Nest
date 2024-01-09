#pragma once

#include <cstdint>

#include "Voxel.hpp"

class Chunk {
private:
    uint32_t mWight;
    uint32_t mHeight;
    uint32_t mDepth;
    uint32_t mSize;
public:
    Voxel* chunk;
    Chunk(uint32_t wight, uint32_t height, uint32_t depth);
    ~Chunk();
    inline uint32_t getSize() const {
        return mSize;
    }
    inline uint32_t getW() const {
        return mWight;
    }
    inline uint32_t getH() const {
        return mHeight;
    }
    inline uint32_t getD() const {
        return mDepth;
    }

    void set(uint32_t x, uint32_t y, uint32_t z, VoxelType type) {
        chunk[y * mWight * mHeight + x * mWight + z].type = type;
    }

   Voxel* get(int32_t x, int32_t y, int32_t z) {
        if (x < 0 || y < 0 || z < 0 || x >= mWight || y >= mHeight || z >= mDepth) {
            return nullptr;
        }

        return &chunk[y * mWight * mHeight + x * mWight + z];
    }

    uint8_t getType(int32_t x, int32_t y, int32_t z) {
        if (x < 0 || y < 0 || z < 0 || x >= mWight || y >= mHeight || z >= mDepth) {
            return 0;
        }

        return uint8_t(chunk[y * mWight * mHeight + x * mWight + z].type);
    }
};