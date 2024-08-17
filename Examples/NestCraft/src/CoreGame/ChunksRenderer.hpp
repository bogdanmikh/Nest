#pragma once

#include <iostream>
#include <mutex>

#include <Nest.hpp>

#include "ChunksStorage.hpp"
#include "GameObject.hpp"
#include "BlocksCreation.hpp"
#include "ChunkMeshGenerator.hpp"
#include "Menu.hpp"

class ChunksRenderer final : public Nest::Entity {
public:
    ChunksRenderer()
        : m_texture("Textures/BlocksTile.png") {}
    ~ChunksRenderer() override;
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override {
        m_menu.update();
    }
    void onDetach() override;
    void draw();

private:
    Menu m_menu;
    Nest::Renderer3D m_renderer3D;
    Bird::ProgramHandle m_shader;
    Nest::Texture m_texture;
    BlocksCreation *m_blocksCreation;
    ChunksStorage *m_chunksStorage;
};