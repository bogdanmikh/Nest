#pragma once

#include <Nest.hpp>
#include "Voxel.hpp"

class Chunk {
public:
    static const uint32_t SIZE_X = 20;
    static const uint32_t SIZE_Y = 20;
    static const uint32_t SIZE_Z = 20;
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