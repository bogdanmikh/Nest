//
// Created by Admin on 25.01.2022.
//

#include "Nest/GameLogic/World.hpp"
#include "Nest/GameLogic/NativeScript.hpp"
#include "Nest/GameLogic/Components/WorldCamera.hpp"
#include "Nest/GameLogic/Components/SkyComponent.hpp"

#include <Rain/Rain.hpp>
#include <entt/entt.hpp>

namespace Nest {

World::World()
    : m_isRunning(false)
    , m_isChanged(false)
    , m_registry() {}

World::~World() {}

void World::updateRuntime(double deltaTime) {
    if (!m_isRunning) {
        return;
    }
    m_renderer2d.begin();
    m_renderer3d.begin();

    glm::mat4 viewProjMtx;
    glm::mat4 skyViewProjMtx;
    Entity cameraEntity = findMainCameraEntity();
    if (cameraEntity.isValid()) {
        WorldCamera &camera = cameraEntity.getComponent<CameraComponent>().camera;

        glm::mat4 viewMtx = glm::inverse(cameraEntity.getTransform().getTransform());
        glm::mat4 skyViewMtx = glm::inverse(cameraEntity.getTransform().getSkyTransform());
        glm::mat4 projMtx = camera.getProjection();

        viewProjMtx = projMtx * viewMtx;
        skyViewProjMtx = projMtx * skyViewMtx;
        m_renderer2d.setViewProj(viewProjMtx);
        m_renderer3d.setViewProj(viewProjMtx);
    }
    updateBasicComponents(deltaTime, viewProjMtx, skyViewProjMtx);
    // Update native scripts
    {
        auto view = m_registry.view<NativeScriptListComponent>();
        for (auto entityHandle : view) {
            if (!m_registry.valid(entityHandle)) {
                continue;
            }
            auto &component = view.get<NativeScriptListComponent>(entityHandle);
            for (auto &container : component.scripts) {
                if (!container.initialized) {
                    id_t entityId = static_cast<id_t>(entityHandle);
                    container.instance->setEntity({&m_registry, entityId, this});
                    container.instance->initialize();
                    container.initialized = true;
                }
                container.instance->update(deltaTime);
            }
        }
    }

    m_renderer2d.end();
    m_renderer3d.end();
}

void World::updateSimulation(double deltaTime, glm::mat4 &viewProjMtx, glm::mat4 &skyViewProjMtx) {
    m_renderer2d.begin();
    m_renderer3d.begin();

    updateBasicComponents(deltaTime, viewProjMtx, skyViewProjMtx);

    // Update native scripts
    {
        auto view = m_registry.view<NativeScriptListComponent>();
        for (auto entityHandle : view) {
            if (!m_registry.valid(entityHandle)) {
                continue;
            }
            auto &component = view.get<NativeScriptListComponent>(entityHandle);
            for (auto &container : component.scripts) {
                if (!container.initialized) {
                    id_t entityId = static_cast<id_t>(entityHandle);
                    container.instance->setEntity({&m_registry, entityId, this});
                    container.instance->initialize();
                    container.initialized = true;
                }
                container.instance->update(deltaTime);
            }
        }
    }

    m_renderer2d.setViewProj(viewProjMtx);
    m_renderer3d.setViewProj(viewProjMtx);

    m_renderer2d.end();
    m_renderer3d.end();
}

void World::updateEditor(double deltaTime, glm::mat4 &viewProjMtx, glm::mat4 &skyViewProjMtx) {
    m_renderer2d.begin();
    m_renderer3d.begin();

    updateBasicComponents(deltaTime, viewProjMtx, skyViewProjMtx);

    m_renderer2d.setViewProj(viewProjMtx);
    m_renderer3d.setViewProj(viewProjMtx);

    m_renderer2d.end();
    m_renderer3d.end();
}

void World::updateBasicComponents(
    float deltaTime, glm::mat4 &viewProjMtx, glm::mat4 &skyViewProjMtx
) {
    // Draw sky
    {
        auto view = m_registry.view<SkyComponent>();
        for (auto &entityHandle : view) {
            if (!m_registry.valid(entityHandle)) {
                continue;
            }
            auto &sky = view.get<SkyComponent>(entityHandle);
            sky.setViewId(m_renderingViewId);
            sky.update(skyViewProjMtx);
        }
    }
    // Render Sprites
    {
        auto view = m_registry.view<SpriteRendererComponent, TransformComponent>();
        for (auto entityHandle : view) {
            if (!m_registry.valid(entityHandle)) {
                continue;
            }
            auto &spriteComponent = view.get<SpriteRendererComponent>(entityHandle);
            auto &transform = view.get<TransformComponent>(entityHandle);
            Nest::Renderer2D::RectData rect;
            rect.color = spriteComponent.color;
            rect.size = {1.f, 1.f};
            rect.center = transform.getPosition();
            rect.rotation = transform.getRotationEuler().z;
            m_renderer2d.drawRect(rect);
        }
    }
    // Render static meshes
    {
        auto view = m_registry.view<StaticMeshComponent, TransformComponent>();
        for (auto entityHandle : view) {
            if (!m_registry.valid(entityHandle)) {
                continue;
            }
            auto &staticMeshComponent = view.get<StaticMeshComponent>(entityHandle);
            auto &transform = view.get<TransformComponent>(entityHandle);
            for (auto &mesh : staticMeshComponent.meshes) {
                m_renderer3d.submit(&transform, &mesh);
            }
        }
    }
    // Render dynamic meshes
    {
        auto view = m_registry.view<DynamicMeshComponent, TransformComponent>();
        for (auto entityHandle : view) {
            if (!m_registry.valid(entityHandle)) {
                continue;
            }
            auto &dynamicMeshComponent = view.get<DynamicMeshComponent>(entityHandle);
            auto &transform = view.get<TransformComponent>(entityHandle);
            for (auto &mesh : dynamicMeshComponent.meshes) {
                m_renderer3d.submit(&transform, &mesh);
            }
        }
    }
}

void World::onImGuiRender() {
    auto view = m_registry.view<NativeScriptListComponent>();
    for (auto entityHandle : view) {
        if (!m_registry.valid(entityHandle)) {
            continue;
        }
        auto &component = view.get<NativeScriptListComponent>(entityHandle);
        for (auto &container : component.scripts) {
            if (container.initialized) {
                container.instance->onImGuiRender();
            }
        }
    }
}

void World::initialize() {
    m_isRunning = true;
    m_isChanged = false;
}

Entity World::instantiateEntity() {
    id_t entityId = static_cast<id_t>(m_registry.create());
    Entity entity = {&m_registry, entityId, this};
    fillEntity(entity);
    m_isChanged = true;
    return entity;
}

Entity World::instantiateEntity(Nest::id_t id) {
    m_registry.create(static_cast<entt::entity>(id));
    Entity entity = {&m_registry, id, this};
    fillEntity(entity);
    m_isChanged = true;
    return entity;
}

void World::fillEntity(Entity entity) {
    entity.addComponent<IdComponent>(entity.m_id);
    entity.addComponent<TagComponent>("Entity");
    entity.addComponent<RelationshipComponent>();
    entity.addComponent<TransformComponent>();
    entity.addComponent<StaticMeshComponent>();
    entity.addComponent<DynamicMeshComponent>();
    entity.addComponent<NativeScriptListComponent>();
}

void World::destroy(Entity entity) {
    entity.removeFromParent();
    for (id_t childHandle : entity.getChildEntities()) {
        Entity child = Entity(&m_registry, childHandle, this);
        if (!child.isValid()) {
            continue;
        }
        destroy(child);
    }
    m_registry.destroy(static_cast<entt::entity>(entity.getId()));
}

void World::clear() {
    for (auto id : m_registry.storage<entt::entity>()) {
        m_registry.destroy(id);
    }
    m_registry.clear();
}

Entity World::findMainCameraEntity() {
    auto view = m_registry.view<CameraComponent>();
    for (auto entity : view) {
        if (!m_registry.valid(entity)) {
            continue;
        }
        auto &comp = view.get<CameraComponent>(entity);
        if (comp.isPrimary) {
            id_t id = static_cast<id_t>(entity);
            return {&m_registry, id, this};
        }
    }
    return {};
}

Camera *World::findMainCamera() {
    Entity entity = findMainCameraEntity();
    if (!entity.isValid()) {
        return nullptr;
    }
    return &entity.getComponent<CameraComponent>().camera;
}

void World::fillStartupData() {
    Entity cameraEntity = instantiateEntity();
    cameraEntity.setName("Camera");
    cameraEntity.getTransform().setPosition({0.f, 0.f, 4.f});
    cameraEntity.addComponent<CameraComponent>();

    Entity sprite1Entity = instantiateEntity();
    sprite1Entity.setName("Orange Sprite");
    auto &sprite1 = sprite1Entity.addComponent<SpriteRendererComponent>();
    sprite1.color = {1.0f, 0.5f, 0.2f, 1.0f};

    Entity skyEntity = instantiateEntity();
    skyEntity.setName("Sky");
    skyEntity.addComponent<SkyComponent>();
    m_isChanged = true;
}

bool World::isChanged() {
    return m_isChanged;
}

void World::resetChanged() {
    m_isChanged = false;
}

void World::setChanged() {
    m_isChanged = true;
}

void World::setViewId(NestRen::ViewId id) {
    m_renderer2d.setViewId(id);
    m_renderer3d.setViewId(id);
    m_renderingViewId = id;
}

} // namespace Nest