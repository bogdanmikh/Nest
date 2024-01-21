#pragma once

#include <iostream>
#include <NestUI.hpp>
#include <Nest.hpp>
#include "ChunkManager.hpp"
#include "ChunkMeshGenerator.hpp"


class ChunksRenderer {
public:
    ChunksRenderer(uint32_t xSize);
    void draw();
private:
    ChunkMeshGenerator chunkMeshGenerator;
    Mesh** meshes;
};

/*
 auto* chunkManager = new ChunkManager(4, 1, 4);
    ChunkMeshGenerator chunkMeshGenerator;
    Mesh** meshes = new Mesh*[chunkManager->getSize()];
    for (int i = 0; i < chunkManager->getSize(); ++i) {
        meshes[i] = chunkMeshGenerator.generateMesh(chunkManager->chunks[i]);
        meshes[i]->addTexture("/home/bogdan/Projects/Nest/Examples/NestCraft/res/textures/BlocksTile.png");
    }
 */