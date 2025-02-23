//
// Created by Bogdan on 06.08.2024.
//

#include "DpsLada.hpp"
#include "Nest/Assets/AssetLoader.hpp"

#include <fstream>

DpsLada::DpsLada() {}

void DpsLada::onAttach() {
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vstModel3D.glsl", "Shaders/fstModel3D.glsl");
    m_shaderHandle = createProgram(programAsset.getBirdProgramCreate());
    m_model.create(
        m_shaderHandle, "Models/lada_2109_dps/scene.gltf", {"material.diffuse", "material.specular"}
    );
}

void DpsLada::onUpdate(double deltaTime) {
    auto camera = Nest::Application::get()->getWorldCamera();
    auto &transform = m_model.getTransform();
    transform.setPosition({15, 4.4, 0});
    transform.setScale({0.5, 0.5, 0.5});

    m_light.update();
    m_light.setUniforms(m_shaderHandle);
    m_model.draw();
}

void DpsLada::onImGuiRender() {}

void DpsLada::onDetach() {}
