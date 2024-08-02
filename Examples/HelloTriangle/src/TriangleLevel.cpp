//
// Created by Admin on 19/03/2022.
//

#include "TriangleLevel.hpp"
#include "TriangleRenderer.hpp"
#include <Nest.hpp>

TriangleLevel::~TriangleLevel() {
    onDetach();
}

void TriangleLevel::onAttach() {
    addEntity(new TriangleRenderer);
    for (auto &entity : m_entities) {
        entity->onAttach();
    }
}

void TriangleLevel::onUpdate(double deltaTime) {
    for (auto &entity : m_entities) {
        entity->onUpdate(deltaTime);
        entity->onImGuiRender();
    }
}

void TriangleLevel::onDetach() {
    for (auto &entity : m_entities) {
        entity->onDetach();
    }
}

void TriangleLevel::addEntity(Nest::Entity *entity) {
    m_entities.emplace_back(entity);
}