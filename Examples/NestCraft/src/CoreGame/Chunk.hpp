#pragma once

#include <Nest.hpp>
#include "Voxel.hpp"

class Chunk {
public:
    static const uint32_t SIZE_X = 30;
    static const uint32_t SIZE_Y = 30;
    static const uint32_t SIZE_Z = 30;
    static const uint32_t SIZE_XYZ = SIZE_X * SIZE_Y * SIZE_Z;
    Chunk();
    ~Chunk();
    void set(int indexX, int indexY, int indexZ, VoxelType type);
    Voxel* get(int indexX, int indexY, int indexZ);
    uint8_t getType(int indexX, int indexY, int indexZ);

    void setMesh(Mesh* mesh);
    Mesh* getMesh();

    Voxel* data;
private:
    Mesh* m_mesh;
};