#include "SnowSceneLevel.hpp"
#include "CameraMove.hpp"
#include "SnowScene.hpp"

#include <Nest.hpp>

void SnowSceneLevel::onAttach() {
    m_carRenderer = new SnowScene;
    m_cameraMove = new CameraMove;
    addEntity(m_carRenderer);
    addEntity(m_cameraMove);
    for (auto &entity : m_entities) {
        entity->onAttach();
    }
}

void SnowSceneLevel::onUpdate(double deltaTime) {
    Bird::setViewClear(0, 0x3D75C9FF);
    for (auto &entity : m_entities) {
        entity->onImGuiRender();
        entity->onUpdate(deltaTime);
    }
}

void SnowSceneLevel::onDetach() {
    for (auto &entity : m_entities) {
        entity->onDetach();
        delete entity;
    }
}

void SnowSceneLevel::addEntity(Nest::Entity *entity) {
    m_entities.emplace_back(entity);
}