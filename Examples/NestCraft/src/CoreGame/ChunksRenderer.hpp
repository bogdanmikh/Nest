#pragma once

#include <iostream>
#include <mutex>

#include "NestUI.hpp"
#include "Nest.hpp"

#include "ChunksStorage.hpp"
#include "ChunkMeshGenerator.hpp"

class ChunksRenderer {
public:
    void init();
    void draw(Window* window, Camera *camera);
    ~ChunksRenderer();
private:
    bool lastDownE;
    bool lastDownQ;
    std::mutex meshesMutex;
    uint32_t w;
    uint32_t h;
    uint32_t d;
    ChunkMeshGenerator chunkMeshGenerator;
    ChunksStorage* chunksStorage;
    Mesh** meshes;
};