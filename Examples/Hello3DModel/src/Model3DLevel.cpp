#include "Model3DLevel.hpp"
#include "CameraMove.hpp"
#include "CarRenderer.hpp"

#include <Nest.hpp>

Model3DLevel::~Model3DLevel() {
    onDetach();
}

void Model3DLevel::onAttach() {
    m_carRenderer = new CarRenderer;
    m_cameraMove = new CameraMove;
    addEntity(m_carRenderer);
    addEntity(m_cameraMove);
    for (auto &entity : m_entities) {
        entity->onAttach();
    }
}

void Model3DLevel::onUpdate(double deltaTime) {
    Bird::setViewClear(0, 0x3D75C9FF);
    for (auto &entity : m_entities) {
        entity->onUpdate(deltaTime);
        entity->onImGuiRender();
    }
}

void Model3DLevel::onDetach() {
    for (auto &entity : m_entities) {
        entity->onDetach();
        delete entity;
    }
}

void Model3DLevel::addEntity(Nest::Entity *entity) {
    m_entities.emplace_back(entity);
}