#pragma once

#include <iostream>
#include <mutex>

#include <NestUI.hpp>
#include <Nest.hpp>

#include "ChunksStorage.hpp"
#include "BlocksCreation.hpp"
#include "ChunkMeshGenerator.hpp"

class ChunksRenderer final : public GameObject {
public:
    ChunksRenderer(){}
    ~ChunksRenderer() override;
    void init();
    void draw() override;
    void update(double deltaTime) override;
private:
    BlocksCreation* blocksCreation;
    ChunksStorage* chunksStorage;
    Mesh** meshes;
};