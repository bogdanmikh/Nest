#pragma once

#include <cstdint>

#include "Voxel.hpp"

class Chunk {
public:
    static const uint32_t wight  = 10;
    static const uint32_t height = 10;
    static const uint32_t depth  = 10;
    Chunk(int posX, int posY, int posZ);
    ~Chunk();
    void set(uint32_t x, uint32_t y, uint32_t z, VoxelType type);
    Voxel* get(int32_t x, int32_t y, int32_t z);
    uint8_t getType(int32_t x, int32_t y, int32_t z);

    int getPosX() const;
    int getPosY() const;
    int getPosZ() const;
private:
    int posX;
    int posY;
    int posZ;
    Voxel* chunk;
};