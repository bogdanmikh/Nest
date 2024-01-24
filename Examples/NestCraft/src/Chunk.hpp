#pragma once

#include "Voxel.hpp"

class Chunk {
public:
    static const uint32_t SIZE_X = 4;
    static const uint32_t SIZE_Y = 4;
    static const uint32_t SIZE_Z = 4;
    Chunk();
    ~Chunk();
    void set(int32_t indexX, int32_t indexY, int32_t indexZ, VoxelType type);
    Voxel* get(int32_t indexX, int32_t indexY, int32_t indexZ);
    uint8_t getType(int32_t indexX, int32_t indexY, int32_t indexZ);
private:
    Voxel* data;
};