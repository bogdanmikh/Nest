#include "SphereLevel.hpp"
#include "SphereRenderer.hpp"
#include "CameraMove.hpp"

#include <Nest.hpp>

SphereLevel::~SphereLevel() {
    onDetach();
}

void SphereLevel::onAttach() {
    addEntity(new CameraMove);
    addEntity(new SphereRenderer);
    for (auto &entity : m_entities) {
        entity->onAttach();
    }
}

void SphereLevel::onUpdate(double deltaTime) {
    for (auto &entity : m_entities) {
        entity->onUpdate(deltaTime);
        entity->onImGuiRender();
    }
}

void SphereLevel::onDetach() {
    for (auto &entity : m_entities) {
        entity->onDetach();
        delete entity;
    }
}

void SphereLevel::addEntity(Nest::Entity *entity) {
    m_entities.emplace_back(entity);
}