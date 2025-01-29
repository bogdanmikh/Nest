#include "FiguresLevel.hpp"
#include "CameraMove.hpp"
#include "CubeRenderer.hpp"
#include "SphereRenderer.hpp"

#include <Nest.hpp>

FigureLevel::~FigureLevel() {}

void FigureLevel::onAttach() {
    m_sphereRenderer = F_NEW(Foundation::getAllocator(), SphereRenderer);
    m_cameraMove = F_NEW(Foundation::getAllocator(), CameraMove);
    m_cubeRenderer = F_NEW(Foundation::getAllocator(), CubeRenderer);
    m_sphereRenderer->getTransform().setPosition({4., 0., 0.});
    SphereRenderer *sphereRenderer = F_NEW(Foundation::getAllocator(), SphereRenderer);
    sphereRenderer->getTransform().setPosition({8., 0., 0.});
    addEntity(m_cameraMove);
    addEntity(m_sphereRenderer);
    addEntity(sphereRenderer);
    addEntity(m_cubeRenderer);
    for (auto &entity : m_entities) {
        entity->onAttach();
    }
}

void FigureLevel::onUpdate(double deltaTime) {
    //    m_cubeRenderer->getTransform().translate(offset);
    auto rotationCube = m_cubeRenderer->getTransform().getRotationEuler();
    rotationCube.x += 3;
    rotationCube.y += 3;
    m_cubeRenderer->getTransform().setRotationEuler(rotationCube);
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