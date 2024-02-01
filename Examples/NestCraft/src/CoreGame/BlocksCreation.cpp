//
// Created by Admin on 12.02.2022.
//

#include "BlocksCreation.hpp"
#include "ChunkMeshGenerator.hpp"
#include "Voxel.hpp"

#include <imgui.h>

void BlocksCreation::init() {
    m_selectedBlock = VoxelType::GROUND;
}

void BlocksCreation::updateChunk(int chunkIndexX, int chunkIndexY, int chunkIndexZ) {
    Mesh* data = ChunkMeshGenerator::generateMesh(m_chunksStorage, chunkIndexX, chunkIndexY, chunkIndexZ, true);
    m_chunksStorage
        ->chunks[chunkIndexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
                 chunkIndexX * ChunksStorage::SIZE_X + chunkIndexZ]
        .setMesh(data);
}

void BlocksCreation::setVoxel(int x, int y, int z, VoxelType type) {
    if (x < 0 || y < 0 || z < 0 || x >= ChunksStorage::WORLD_SIZE_X ||
        y >= ChunksStorage::WORLD_SIZE_Y || z >= ChunksStorage::WORLD_SIZE_Z)
        return;
    m_chunksStorage->setVoxel(x, y, z, type);
    int chunkIndexX = x / Chunk::SIZE_X;
    int chunkIndexY = y / Chunk::SIZE_Y;
    int chunkIndexZ = z / Chunk::SIZE_Z;
    updateChunk(chunkIndexX, chunkIndexY, chunkIndexZ);

    if (x % Chunk::SIZE_X == 0 && chunkIndexX > 0) {
        updateChunk(chunkIndexX - 1, chunkIndexY, chunkIndexZ);
    }
    if (x % Chunk::SIZE_X == Chunk::SIZE_X - 1 && chunkIndexX < ChunksStorage::SIZE_X - 1) {
        updateChunk(chunkIndexX + 1, chunkIndexY, chunkIndexZ);
    }
    if (z % Chunk::SIZE_Z == 0 && chunkIndexZ > 0) {
        updateChunk(chunkIndexX, chunkIndexY, chunkIndexZ - 1);
    }
    if (z % Chunk::SIZE_Z == Chunk::SIZE_Z - 1 && chunkIndexZ < ChunksStorage::SIZE_Z - 1) {
        updateChunk(chunkIndexX, chunkIndexY, chunkIndexZ + 1);
    }
    if (y % Chunk::SIZE_Y == 0 && chunkIndexY > 0) {
        updateChunk(chunkIndexX, chunkIndexY - 1, chunkIndexZ);
    }
    if (y % Chunk::SIZE_Y == Chunk::SIZE_Y - 1 && chunkIndexY < ChunksStorage::SIZE_Y - 1) {
        updateChunk(chunkIndexX, chunkIndexY + 1, chunkIndexZ);
    }
}

void BlocksCreation::update(double deltaTime) {
    bool leftPressed;
    bool rightPressed;
    leftPressed = Application::getInstance()->getWindow()->isMouseButtonPressed(MouseButton::LEFT);
    rightPressed = Application::getInstance()->getWindow()->isMouseButtonPressed(MouseButton::RIGHT);
    bool canCreateVoxel = leftPressed && !lastPressLeftMouseButton;
    bool canDeleteVoxel = rightPressed && !lastPressRightMouseButton;
    lastPressLeftMouseButton = leftPressed;
    lastPressRightMouseButton = rightPressed;
    if (!leftPressed && !rightPressed) {
        return;
    }
    glm::vec3 position = m_camera->getPosition();
    glm::vec3 target = m_camera->getFront();
    auto v = m_chunksStorage->bresenham3D(
        position.x, position.y, position.z, target.x, target.y, target.z, MAXIMUM_DISTANCE);
    if (v && v->voxel != nullptr) {
        if (canCreateVoxel) {
            int x = v->end.x + v->normal.x;
            int y = v->end.y + v->normal.y;
            int z = v->end.z + v->normal.z;
            setVoxel(x, y, z, m_selectedBlock);
        } else if (canDeleteVoxel) {
            int x = v->end.x;
            int y = v->end.y;
            int z = v->end.z;
            setVoxel(x, y, z, VoxelType::NOTHING);
        }
    }
}

void BlocksCreation::onImGuiRender() {
    ImGui::Begin("Block");
    if (ImGui::Button("TREE")) {
        m_selectedBlock = VoxelType::TREE;
    }
    if (ImGui::Button("STONE BRICKS")) {
        m_selectedBlock = VoxelType::STONE_BRICKS;
    }
    if (ImGui::Button("BOARDS")) {
        m_selectedBlock = VoxelType::BOARDS;
    }
    ImGui::End();
}

void BlocksCreation::setChunksStorage(ChunksStorage* storage) {
    m_chunksStorage = storage;
}

void BlocksCreation::setCamera(Camera *camera) {
    m_camera = camera;
}
