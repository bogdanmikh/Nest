#include "BlocksCreation.hpp"
#include "ChunkMeshGenerator.hpp"
#include "Voxel.hpp"

#include <imgui.h>

void BlocksCreation::init(Bird::ProgramHandle shaderHandle, Bird::TextureHandle textureHandle) {
    m_shader = shaderHandle;
    m_selectedBlock = VoxelType(11);
    m_textureHandle = textureHandle;
}

void BlocksCreation::updateChunk(int chunkIndexX, int chunkIndexY, int chunkIndexZ) {
    Nest::StaticMesh *data = ChunkMeshGenerator::generateMesh(
        m_textureHandle, m_shader, m_chunksStorage, chunkIndexX, chunkIndexY, chunkIndexZ, true
    );
    m_chunksStorage
        ->chunks
            [chunkIndexY * ChunksStorage::SIZE_X * ChunksStorage::SIZE_Z +
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
    updateVoxelHand();
    bool leftPressed;
    bool rightPressed;
    leftPressed = Nest::Input::isMouseButtonJustPressed(Nest::MouseButton::LEFT) &&
                  Nest::Application::get()->getWindow()->isCursorLocked();
    rightPressed = Nest::Input::isMouseButtonJustPressed(Nest::MouseButton::RIGHT) &&
                   Nest::Application::get()->getWindow()->isCursorLocked();
    if (!leftPressed && !rightPressed) {
        return;
    }
    glm::vec3 position = m_camera->getPosition();
    glm::vec3 target = m_camera->getFront();
    auto v = m_chunksStorage->bresenham3D(
        position.x, position.y, position.z, target.x, target.y, target.z, MAXIMUM_DISTANCE
    );
    if (v && v->voxel != nullptr) {
        if (leftPressed) {
            int x = v->end.x + v->normal.x;
            int y = v->end.y + v->normal.y;
            int z = v->end.z + v->normal.z;
            setVoxel(x, y, z, m_selectedBlock);
        } else if (rightPressed) {
            int x = v->end.x;
            int y = v->end.y;
            int z = v->end.z;
            setVoxel(x, y, z, VoxelType::NOTHING);
        }
    }
}

void BlocksCreation::onImGuiRender() {
#ifndef PLATFORM_ANDROID
    ImGui::SetNextWindowPos(ImVec2(500.0f, 500.0f));
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
#else
    float coeff = 0.5;
    ImGui::SetNextWindowPos({600, 600});
    ImGui::Begin("Create", nullptr);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    if (ImGui::Button("##create", {200, 200})) {
        glm::vec3 position = m_camera->getPosition();
        glm::vec3 target = m_camera->getFront();
        auto v = m_chunksStorage->bresenham3D(
                position.x, position.y, position.z, target.x, target.y, target.z, MAXIMUM_DISTANCE
        );
        if (v && v->voxel != nullptr) {
            int x = v->end.x + v->normal.x;
            int y = v->end.y + v->normal.y;
            int z = v->end.z + v->normal.z;
            setVoxel(x, y, z, m_selectedBlock);
        }
    }
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    if (ImGui::Button("##delete", {200, 200})) {
        glm::vec3 position = m_camera->getPosition();
        glm::vec3 target = m_camera->getFront();
        auto v = m_chunksStorage->bresenham3D(
                position.x, position.y, position.z, target.x, target.y, target.z, MAXIMUM_DISTANCE
        );
        if (v && v->voxel != nullptr) {
            int x = v->end.x;
            int y = v->end.y;
            int z = v->end.z;
            setVoxel(x, y, z, VoxelType::NOTHING);
        }
    }

    ImGui::PopStyleColor();
    ImGui::End();
#endif
}

void BlocksCreation::updateVoxelHand() {
    if (Nest::Input::isKeyJustPressed(Nest::Key::KEY_1)) {
        m_selectedBlock = VoxelType(1);
    } else if (Nest::Input::isKeyJustPressed(Nest::Key::KEY_2)) {
        m_selectedBlock = VoxelType(7);
    } else if (Nest::Input::isKeyJustPressed(Nest::Key::KEY_3)) {
        m_selectedBlock = VoxelType(8);
    } else if (Nest::Input::isKeyJustPressed(Nest::Key::KEY_4)) {
        m_selectedBlock = VoxelType(9);
    } else if (Nest::Input::isKeyJustPressed(Nest::Key::KEY_5)) {
        m_selectedBlock = VoxelType(10);
    } else if (Nest::Input::isKeyJustPressed(Nest::Key::KEY_6)) {
        m_selectedBlock = VoxelType(11);
    } else if (Nest::Input::isKeyJustPressed(Nest::Key::KEY_7)) {
        m_selectedBlock = VoxelType(12);
    }
}

void BlocksCreation::setChunksStorage(ChunksStorage *storage) {
    m_chunksStorage = storage;
}

void BlocksCreation::setCamera(Nest::WorldCamera *camera) {
    m_camera = camera;
}
