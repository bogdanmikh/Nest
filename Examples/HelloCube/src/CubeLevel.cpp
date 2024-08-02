#include "CubeLevel.hpp"
#include "CubeRenderer.hpp"
#include "CameraMove.hpp"

#include <Nest.hpp>

CubeLevel::~CubeLevel() {
    onDetach();
}

void CubeLevel::onAttach() {
    addEntity(new CameraMove);
    addEntity(new CubeRenderer);
    for (auto &entity : m_entities) {
        entity->onAttach();
    }
}

void CubeLevel::onUpdate(double deltaTime) {
    for (auto &entity : m_entities) {
        entity->onUpdate(deltaTime);
        entity->onImGuiRender();
    }
}

void CubeLevel::onDetach() {
    for (auto &entity : m_entities) {
        entity->onDetach();
        delete entity;
    }
}

void CubeLevel::addEntity(Nest::Entity *entity) {
    m_entities.emplace_back(entity);
}