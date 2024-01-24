#include "ChunksStorage.hpp"

ChunksStorage::~ChunksStorage() {
    delete[] chunks;
}

ChunksStorage::ChunksStorage() {
    chunks = new Chunk*[SIZE_X * SIZE_Y * SIZE_Z];
    uint32_t index = 0;
    for (int x = 0; x < SIZE_X; ++x) {
        for (int y = 0; y < SIZE_Y; ++y) {
            for (int z = 0; z < SIZE_Z; ++z) {
                chunks[index++] = new Chunk();
            }
        }
    }
}

void ChunksStorage::setVoxel(int x, int y, int z, VoxelType type) {
    if (x < 0 || y < 0 || z < 0 || x >= WORLD_SIZE_X || y >= WORLD_SIZE_Y || z >= WORLD_SIZE_Z)
        return;
    int chunkIndexX = x / Chunk::SIZE_X;
    int chunkIndexY = y / Chunk::SIZE_Y;
    int chunkIndexZ = z / Chunk::SIZE_Z;
    chunks[chunkIndexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
           chunkIndexX * ChunksStorage::SIZE_X + chunkIndexZ]
            ->set(x % Chunk::SIZE_X, y % Chunk::SIZE_Y, z % Chunk::SIZE_Z, type);
}

Voxel *ChunksStorage::getVoxel(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0 || x >= WORLD_SIZE_X || y >= WORLD_SIZE_Y || z >= WORLD_SIZE_Z)
        return nullptr;
    int chunkIndexX = x / Chunk::SIZE_X;
    int chunkIndexY = y / Chunk::SIZE_Y;
    int chunkIndexZ = z / Chunk::SIZE_Z;
    return chunks[chunkIndexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                  chunkIndexX * ChunksStorage::SIZE_X + chunkIndexZ]
            ->get(x % Chunk::SIZE_X, y % Chunk::SIZE_Y, z % Chunk::SIZE_Z);
}

Chunk *ChunksStorage::getChunk(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0 || x >= WORLD_SIZE_X || y >= WORLD_SIZE_Y || z >= WORLD_SIZE_Z)
        return nullptr;
    int chunkIndexX = x / Chunk::SIZE_X;
    int chunkIndexY = y / Chunk::SIZE_Y;
    int chunkIndexZ = z / Chunk::SIZE_Z;
    return chunks[chunkIndexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                   chunkIndexX * ChunksStorage::SIZE_X + chunkIndexZ];
}


Chunk *ChunksStorage::getChunkIndex(int index) {
    return chunks[index];
}
