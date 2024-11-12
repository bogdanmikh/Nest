//
// Created by Bogdan on 06.08.2024.
//

#include "StreetLamp.hpp"
#include "Nest/Assets/AssetLoader.hpp"

#include <fstream>

StreetLamp::StreetLamp() {}

void StreetLamp::onAttach() {
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vstModel3D.glsl", "Shaders/fstModel3D.glsl");
    m_shaderHandle = createProgram(programAsset.getBirdProgramCreate());
    m_model.create(m_shaderHandle, "Models/street_lamp/scene.gltf");
}

void StreetLamp::onUpdate(double deltaTime) {
    auto &transform = m_model.getTransform();
    transform.setPosition({15, -1, 2});
    //    transform.setScale({10., 10., 10.});
    transform.setRotationEuler({-90, 0, 0});
    transform.setScale({0.04, 0.04, 0.04});
    m_light.update();
    m_light.setUniforms(m_shaderHandle);
    m_model.draw();
}

void StreetLamp::onImGuiRender() {}

void StreetLamp::onDetach() {}
