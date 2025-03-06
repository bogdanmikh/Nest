#include "HelloLightLevel.hpp"
#include "CameraMove.hpp"
#include "NightSky.hpp"

#include <Nest.hpp>

void HelloLightLevel::onAttach() {
    addEntity(new NightSky);
    addEntity(new CameraMove);

    InfoLightObject info;
    // old lada
    info.position = {0, 0, 0};
    info.degrees = {0, 0, 0};
    info.scale = {0.1, 0.1, 0.1};
    info.pathToModel = "Models/old_lada/scene.gltf";
    info.createInfoModel3D = {"material.diffuse", "material.specular"};
    addLightObject(info);

    // asphalt
    info.position = {0, -0.2, 7};
    info.degrees = {-90, 90, 0};
    info.scale = {25, 25, 25};
    info.pathToModel = "Models/asphalt/scene.gltf";
    info.createInfoModel3D = {"material.diffuse", "material.specular"};
    addLightObject(info);

    // street lamp
    info.position = {-8, -1, 2};
    info.degrees = {-90, 180, 0};
    info.scale = {0.04, 0.04, 0.04};
    info.pathToModel =  "Models/street_lamp/scene.gltf";
    info.createInfoModel3D = {"material.diffuse", "material.specular"};
//    addLightObject(info);

    // dps
    info.position = {15, 4.4, 0};
    info.degrees = {0, 0, 0};
    info.scale = {0.5, 0.5, 0.5};
    info.pathToModel ="Models/lada_2109_dps/scene.gltf";
    info.createInfoModel3D = {"material.diffuse", "material.specular"};
//    addLightObject(info);

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

void HelloLightLevel::addLightObject(const InfoLightObject &info) {
    LightObject *lightObject = new LightObject;
    lightObject->setInfoLightObject(info);
    addEntity(lightObject);
}

void HelloLightLevel::addEntity(Nest::Entity *entity) {
    m_entities.emplace_back(entity);
}