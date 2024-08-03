#include "FiguresLevel.hpp"
#include "SphereRenderer.hpp"
#include "CubeRenderer.hpp"
#include "CameraMove.hpp"

#include <Nest.hpp>

FigureLevel::~FigureLevel() {
    onDetach();
}

void FigureLevel::onAttach() {
    addEntity(new CameraMove);
    addEntity(new SphereRenderer);
    addEntity(new CubeRenderer);
    for (auto &entity : m_entities) {
        entity->onAttach();
    }
}

void FigureLevel::onUpdate(double deltaTime) {
    for (auto &entity : m_entities) {
        entity->onUpdate(deltaTime);
        entity->onImGuiRender();
    }
}

void FigureLevel::onDetach() {
    for (auto &entity : m_entities) {
        entity->onDetach();
        delete entity;
    }
}

void FigureLevel::addEntity(Nest::Entity *entity) {
    m_entities.emplace_back(entity);
}