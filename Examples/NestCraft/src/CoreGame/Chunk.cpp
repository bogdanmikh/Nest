#include "Chunk.hpp"
#include "ChunksStorage.hpp"

#include "glm/gtc/noise.hpp"
#include <cmath>
#include <random>

void Chunk::init() {
    m_mesh = nullptr;
    data = (Voxel *)F_ALLOC(Foundation::getAllocator(), sizeof(Voxel) * SIZE_X * SIZE_Y * SIZE_Z);
    for (int i = 0; i < SIZE_X * SIZE_Y * SIZE_Z; ++i) {
        data[i] = Voxel();
    }
}

void Chunk::detach() {
    F_DELETE(Foundation::getAllocator(), m_mesh);
    F_DELETE(Foundation::getAllocator(), data);
}

void Chunk::set(int x, int y, int z, VoxelType type) {
    if (x < 0 || y < 0 || z < 0 || x >= SIZE_X || y >= SIZE_Y || z >= SIZE_Z)
        return;
    data[y * Chunk::SIZE_X * Chunk::SIZE_Z + x * Chunk::SIZE_X + z].type = type;
}

Voxel *Chunk::get(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0 || x >= SIZE_X || y >= SIZE_Y || z >= SIZE_Z) {
        return nullptr;
    }
    return &data[y * Chunk::SIZE_X * Chunk::SIZE_Z + x * Chunk::SIZE_X + z];
}

uint8_t Chunk::getType(int indexX, int indexY, int indexZ) {
    Voxel *voxel = get(indexX, indexY, indexZ);
    if (voxel == nullptr) {
        return 0;
    }
    return uint8_t(voxel->type);
}

void Chunk::setMesh(Nest::StaticMesh *mesh) {
    F_DELETE(Foundation::getAllocator(), m_mesh);
    m_mesh = mesh;
}

Nest::StaticMesh *Chunk::getMesh() {
    return m_mesh;
}
