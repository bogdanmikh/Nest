#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "Chunk.hpp"
#include "VoxelRaycastData.hpp"

class ChunksStorage {
public:
    static const int SIZE_X = 1;
    static const int SIZE_Y = 1;
    static const int SIZE_Z = 1;
    static const int SIZE_XYZ =
        ChunksStorage::SIZE_X * ChunksStorage::SIZE_Y * ChunksStorage::SIZE_Z;

    static const int WORLD_SIZE_X = ChunksStorage::SIZE_X * Chunk::SIZE_X;
    static const int WORLD_SIZE_Y = ChunksStorage::SIZE_Y * Chunk::SIZE_Y;
    static const int WORLD_SIZE_Z = ChunksStorage::SIZE_Z * Chunk::SIZE_Z;

    ChunksStorage();
    ~ChunksStorage();
    void setVoxel(int x, int y, int z, VoxelType type);
    Voxel *getVoxel(int x, int y, int z);
    Chunk *getChunk(int indexX, int indexY, int indexZ);
    Chunk *getChunkIndex(int index);

    std::optional<VoxelRaycastData>
    bresenham3D(float x1, float y1, float z1, float x2, float y2, float z2, int maximumDistance);
    Chunk *chunks;

    void saveWorld(unsigned char *data);
    void loadWorld(unsigned char *data);
};
