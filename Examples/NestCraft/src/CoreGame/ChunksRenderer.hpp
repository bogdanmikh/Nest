#pragma once

#include <iostream>
#include <mutex>

#include <Nest.hpp>

#include "BlocksCreation.hpp"
#include "ChunkMeshGenerator.hpp"
#include "ChunksStorage.hpp"
#include "GameObject.hpp"
#include "Menu.hpp"

class ChunksRenderer final : public Nest::Entity {
public:
    ChunksRenderer() {}
    void setFbViewId(Bird::ViewId viewId) {
        m_viewId = viewId;
    }
    ~ChunksRenderer() override;
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override {
        m_menu.update();
        m_blocksCreation->onImGuiRender();
    }
    void onDetach() override;
    void draw();

private:
    Menu m_menu;
    Nest::Renderer3D m_renderer3D;
    Bird::ProgramHandle m_shader;
    Bird::TextureHandle m_texture;
    Bird::ViewId m_viewId;
    BlocksCreation *m_blocksCreation;
    ChunksStorage *m_chunksStorage;
};