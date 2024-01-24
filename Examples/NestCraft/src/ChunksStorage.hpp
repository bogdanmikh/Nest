#pragma once

#include <cstdint>
#include "Chunk.hpp"

class ChunksStorage {
public:
    static const int SIZE_X = 10;
    static const int SIZE_Y = 4;
    static const int SIZE_Z = 10;
    static const int SIZE_XYZ = SIZE_X * SIZE_Y * SIZE_Z;

    static const int WORLD_SIZE_X = ChunksStorage::SIZE_X * Chunk::SIZE_X;
    static const int WORLD_SIZE_Y = ChunksStorage::SIZE_Y * Chunk::SIZE_Y;
    static const int WORLD_SIZE_Z = ChunksStorage::SIZE_Z * Chunk::SIZE_Z;

    ChunksStorage();
    ~ChunksStorage();
    void setVoxel(int x, int y, int z, VoxelType type);
    Voxel* getVoxel(int x, int y, int z);
    Chunk* getChunk(int indexX, int indexY, int indexZ);
    Chunk* getChunkIndex(int index);
private:
    Chunk** chunks;
};
