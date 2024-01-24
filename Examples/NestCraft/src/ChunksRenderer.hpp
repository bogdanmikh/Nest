#pragma once

#include <iostream>
#include <NestUI.hpp>
#include <Nest.hpp>
#include "ChunksStorage.hpp"
#include "ChunkMeshGenerator.hpp"

class ChunksRenderer {
public:
    ChunksRenderer(uint32_t w = 4, uint32_t h = 1, uint32_t d = 4);
    void init();
    void draw();
    ~ChunksRenderer();
private:
    uint32_t w;
    uint32_t h;
    uint32_t d;
    ChunkMeshGenerator chunkMeshGenerator;
    ChunksStorage* chunksStorage;
    Mesh** meshes;
};