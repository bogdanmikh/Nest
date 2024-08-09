//
// Created by Admin on 12.02.2022.
//

#pragma once

#include <Nest.hpp>

#include "ChunksStorage.hpp"
#include "GameObject.hpp"

class BlocksCreation final : public GameObject {
public:
    const int MAXIMUM_DISTANCE = 100;

    void init();
    void update(double deltaTime) override;
    void draw() override {}
    void onImGuiRender();
    void setChunksStorage(ChunksStorage *storage);
    void setCamera(Nest::WorldCamera *camera);

private:
    void setVoxel(int x, int y, int z, VoxelType type);
    void updateChunk(int chunkIndexX, int chunkIndexY, int chunkIndexZ);

    void updateVoxelHand();
    ChunksStorage *m_chunksStorage;
    Nest::WorldCamera *m_camera;
    VoxelType m_selectedBlock;
};
