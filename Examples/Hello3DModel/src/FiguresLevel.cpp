#include "FiguresLevel.hpp"
#include "SphereRenderer.hpp"
#include "CubeRenderer.hpp"
#include "CameraMove.hpp"

#include <Nest.hpp>

FigureLevel::~FigureLevel() {
    onDetach();
}

void FigureLevel::onAttach() {
    m_sphereRenderer = new SphereRenderer;
    m_cameraMove = new CameraMove;
    m_cubeRenderer = new CubeRenderer;
    m_sphereRenderer->getTransform().setPosition({4., 0., 0.});
    addEntity(m_sphereRenderer);
    addEntity(m_cameraMove);
    addEntity(m_cubeRenderer);
    for (auto &entity : m_entities) {
        entity->onAttach();
    }
}

void FigureLevel::onUpdate(double deltaTime) {
    //    m_cubeRenderer->getTransform().translate(offset);
    m_sphereRenderer->getTransform().setRotationEuler({0., 10., 0});
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