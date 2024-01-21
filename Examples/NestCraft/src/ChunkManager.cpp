#include "ChunkManager.hpp"

#include <iostream>

ChunkManager::ChunkManager(uint32_t w, uint32_t h, uint32_t d) : w(w), h(h), d(d), size(w*h*d){
    chunks = new Chunk*[size];
    uint32_t index = 0;
    for (int y = 0; y < h; ++y) {
        for (int z = 0; z < d; ++z) {
            for (int x = 0; x < w; ++x) {
                auto* chunk = new Chunk(w,y, z);
                chunks[index++] = chunk;
            }
        }
    }
}

ChunkManager::~ChunkManager() {
    for (int i = 0; i < size; ++i) {
        delete chunks[i];
    }
    delete[] chunks;
}

uint32_t ChunkManager::getSize() {
    return size;
}

uint32_t ChunkManager::getSizeX() const {
    return w;
}

uint32_t ChunkManager::getSizeY() const {
    return h;
}

uint32_t ChunkManager::getSizeZ() const {
    return d;
}
