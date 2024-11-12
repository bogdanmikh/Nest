//
// Created by Bogdan on 06.08.2024.
//

#include "Asphalt.hpp"
#include "Nest/Assets/AssetLoader.hpp"

#include <fstream>

Asphalt::Asphalt() {}

void Asphalt::onAttach() {
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vstModel3D.glsl", "Shaders/fstModel3D.glsl");
    m_shaderHandle = createProgram(programAsset.getBirdProgramCreate());
    m_model.create(m_shaderHandle, "Models/asphalt/scene.gltf");
}

void Asphalt::onUpdate(double deltaTime) {
    auto &transform = m_model.getTransform();
    transform.setPosition({0, -0.2, 7});
    transform.setRotationEuler({-90, 90, 0});
    float coeffScale = 25.;
    transform.setScale({coeffScale, coeffScale, coeffScale});
    //    transform.setScale({0.05, 0.05, 0.05});
    m_light.update();
    m_light.setUniforms(m_shaderHandle);
    m_model.draw();
}

void Asphalt::onImGuiRender() {}

void Asphalt::onDetach() {}
