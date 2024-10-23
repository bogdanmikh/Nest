#include "HelloLightLevel.hpp"
#include "CameraMove.hpp"
#include "HelloLight.hpp"
#include "NightSky.hpp"

#include <Nest.hpp>

void HelloLightLevel::onAttach() {
    //  m_carRenderer = new SnowScene;
    addEntity(new NightSky);
    addEntity(new HelloLight);
    addEntity(new CameraMove);
    for (auto &entity : m_entities) {
        entity->onAttach();
    }
}

void HelloLightLevel::onUpdate(double deltaTime) {
    Bird::setViewClear(0, 0x3D75C9FF);
    for (auto &entity : m_entities) {
        entity->onImGuiRender();
        entity->onUpdate(deltaTime);
    }
}

void HelloLightLevel::onDetach() {
    for (auto &entity : m_entities) {
        entity->onDetach();
        delete entity;
    }
}

void HelloLightLevel::addEntity(Nest::Entity *entity) {
    m_entities.emplace_back(entity);
}