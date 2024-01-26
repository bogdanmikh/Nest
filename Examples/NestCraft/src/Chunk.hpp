#pragma once

#include "Voxel.hpp"

class Chunk {
public:
    static const uint32_t SIZE_X = 50;
    static const uint32_t SIZE_Y = 50;
    static const uint32_t SIZE_Z = 50;
    Chunk();
    ~Chunk();
    void set(int indexX, int indexY, int indexZ, VoxelType type);
    Voxel* get(int indexX, int indexY, int indexZ);
    uint8_t getType(int indexX, int indexY, int indexZ);

    bool needUpdate;
private:
    Voxel* data;
};