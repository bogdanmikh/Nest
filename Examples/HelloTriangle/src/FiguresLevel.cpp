#include "FiguresLevel.hpp"
#include "TriangleRenderer.hpp"

#include <Nest.hpp>

FigureLevel::~FigureLevel() {}

void FigureLevel::onAttach() {
    m_triangleRenderer = F_NEW(Foundation::getAllocator(), TriangleRenderer);

    addEntity(m_triangleRenderer);
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
        F_DELETE(Foundation::getAllocator(), entity);
    }
}

void FigureLevel::addEntity(Nest::Entity *entity) {
    m_entities.emplace_back(entity);
}