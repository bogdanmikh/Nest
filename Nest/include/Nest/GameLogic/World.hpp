//
// Created by Admin on 25.01.2022.
//

#pragma once

#include "Nest/GameLogic/Entity.hpp"
#include "Nest/Window/Window.hpp"
#include "Nest/Renderer/Renderer2D.hpp"
#include "Nest/Renderer/Renderer3D.hpp"
#include "Camera.hpp"

#include <NestRen/NestRen.hpp>
#include <entt/entt.hpp>

namespace Nest {

class World final {
public:
    World();
    World(World &world) = delete;
    ~World();
    void initialize();
    void fillStartupData();
    void updateRuntime(double deltaTime);
    void updateSimulation(double deltaTime, glm::mat4 &viewProjMtx, glm::mat4 &skyViewProjMtx);
    void updateEditor(double deltaTime, glm::mat4 &viewProjMtx, glm::mat4 &skyViewProjMtx);
    void onImGuiRender();
    Entity instantiateEntity();
    void destroy(Entity entity);
    void clear();
    bool isChanged();
    void resetChanged();
    void setChanged();
    inline bool isRunning() {
        return m_isRunning;
    }
    Renderer2D &getRenderer2D() {
        return m_renderer2d;
    }
    Renderer3D &getRenderer3D() {
        return m_renderer3d;
    }
    void setViewId(NestRen::ViewId id);
    Camera *findMainCamera();
    World &operator=(World &other) = delete;

private:
    void updateBasicComponents(float deltaTime, glm::mat4 &viewProjMtx, glm::mat4 &skyViewProjMtx);
    Entity findMainCameraEntity();
    Entity instantiateEntity(id_t id);
    void fillEntity(Entity entity);

    bool m_isRunning;
    bool m_isChanged;
    entt::registry m_registry;
    Renderer2D m_renderer2d;
    Renderer3D m_renderer3d;
    NestRen::ViewId m_renderingViewId;

    friend class WorldHierarchyPanel;
    friend class WorldMapper;
};

} // namespace Panda