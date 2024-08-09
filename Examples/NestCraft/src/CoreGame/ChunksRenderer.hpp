#pragma once

#include <iostream>
#include <mutex>

#include <Nest.hpp>

#include "ChunksStorage.hpp"
#include "GameObject.hpp"
#include "BlocksCreation.hpp"
#include "ChunkMeshGenerator.hpp"

class ChunksRenderer final : public Nest::Entity {
public:
    ChunksRenderer();
    ~ChunksRenderer() override;
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override {}
    void onDetach() override;
    void draw();

private:
    BlocksCreation *blocksCreation;
    ChunksStorage *chunksStorage;
};