#pragma once

#include "Voxel.hpp"
#include <Nest.hpp>

class Chunk {
public:
    static const uint32_t SIZE_X = 30;
    static const uint32_t SIZE_Y = 30;
    static const uint32_t SIZE_Z = 30;
    static const uint32_t SIZE_XYZ = SIZE_X * SIZE_Y * SIZE_Z;
    void detach();
    void init();
    void set(int indexX, int indexY, int indexZ, VoxelType type);
    Voxel *get(int indexX, int indexY, int indexZ);
    uint8_t getType(int indexX, int indexY, int indexZ);

    void setMesh(Nest::StaticMesh *mesh);
    Nest::StaticMesh *getMesh();

    Voxel *data;

private:
    Nest::StaticMesh *m_mesh;
};