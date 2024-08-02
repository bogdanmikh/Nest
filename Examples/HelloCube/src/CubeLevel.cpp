//
// Created by Admin on 19/03/2022.
//

#include "CubeRenderer.hpp"
#include "CubeLevel.hpp"
#include <Nest.hpp>

CubeLevel::~CubeLevel() {
    onDetach();
}

void CubeLevel::onAttach() {
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
    }
}

void CubeLevel::addEntity(Nest::Entity *entity) {
    m_entities.emplace_back(entity);
}