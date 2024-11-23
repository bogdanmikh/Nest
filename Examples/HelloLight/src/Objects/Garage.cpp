//
// Created by Bogdan on 06.08.2024.
//

#include "Garage.hpp"
#include "Nest/Assets/AssetLoader.hpp"

#include <fstream>

Garage::Garage() {}

void Garage::onAttach() {
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vstModel3D.glsl", "Shaders/fstModel3D.glsl");
    m_shaderHandle = createProgram(programAsset.getBirdProgramCreate());
    m_model.create(m_shaderHandle, "Models/garage/scene.gltf");
}

void Garage::onUpdate(double deltaTime) {
    auto &transform = m_model.getTransform();
    transform.setPosition({-15, 15, 15});
    transform.setRotationEuler({0, 90, 0});
    transform.setScale({0.1, 0.1, 0.1});
    m_light.update();
    m_light.setUniforms(m_shaderHandle);
    m_model.draw();
}

void Garage::onImGuiRender() {}

void Garage::onDetach() {}
