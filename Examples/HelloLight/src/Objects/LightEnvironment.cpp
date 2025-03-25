//
// Created by Bogdan on 24.03.2025.
//

#include "LightEnvironment.hpp"
#include "LightObject.hpp"

void LightEnvironment::initObjects() {
    InfoObject info;
    // old lada
    info.position = {0, 0, 0};
    info.degrees = {0, 0, 0};
    info.scale = {0.1, 0.1, 0.1};
    info.pathToModel = "Models/old_lada/scene.gltf";
    info.createInfoModel3D = {"material.diffuse", "material.specular"};
    // m_managerObjects.add(info);

    // asphalt
    info.position = {0, -0.2, 7};
    info.degrees = {-90, 90, 0};
    info.scale = {25, 25, 25};
    info.pathToModel = "Models/asphalt/scene.gltf";
    info.createInfoModel3D = {"material.diffuse", "material.specular"};
    m_managerObjects.add(info);

    // street lamp
    info.position = {-8, -1, 2};
    info.degrees = {-90, 180, 0};
    info.scale = {0.04, 0.04, 0.04};
    info.pathToModel = "Models/street_lamp/scene.gltf";
    info.createInfoModel3D = {"material.diffuse", "material.specular"};
//    m_managerObjects.add(info);

    // dps
    info.position = {15, 4.4, 0};
    info.degrees = {0, 0, 0};
    info.scale = {0.5, 0.5, 0.5};
    info.pathToModel = "Models/lada_2109_dps/scene.gltf";
    info.createInfoModel3D = {"material.diffuse", "material.specular"};
    //            m_managerObjects.add(info);

    info.position = {0, 0, 0};
    info.degrees = {0, 0, 0};
    info.scale = {0.1, 0.1, 0.1};
    info.pathToModel = "Models/Sponza/sponza.obj";
    info.createInfoModel3D = {"material.diffuse", "material.specular"};
    //            m_managerObjects.add(info);
}

void LightEnvironment::onAttach() {
    initObjects();
    Nest::ProgramAsset programAsset = Nest::AssetLoader::loadProgram(
        "Shaders/vstModelDebug.glsl", "Shaders/fstModelDebug.glsl"
    );
    m_shader = createProgram(programAsset.getBirdProgramCreate());

    for (auto &model: m_managerObjects.getModels()) {
        model.setShader(m_shader);
    }
}

void LightEnvironment::onUpdate(double deltaTime) {
    for (auto &model: m_managerObjects.getModels()) {
        model.draw();
    }
}

void LightEnvironment::onImGuiRender() {

}

void LightEnvironment::onDetach() {
    deleteProgram(m_shader);
}