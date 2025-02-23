//
// Created by Bogdan on 06.08.2024.
//

#include "Lada2110.hpp"
#include "Nest/Assets/AssetLoader.hpp"

#include <fstream>

Lada2110::Lada2110() {}

void Lada2110::onAttach() {
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vstModel3D.glsl", "Shaders/fstModel3D.glsl");
    m_shaderHandle = createProgram(programAsset.getBirdProgramCreate());
    m_model.create(
        m_shaderHandle, "Models/old_lada/scene.gltf", {"material.diffuse", "material.specular"}
    );
}

void Lada2110::onUpdate(double deltaTime) {
    auto camera = Nest::Application::get()->getWorldCamera();
    auto &transform = m_model.getTransform();
    transform.setPosition({0, 0, 0});
    transform.setScale({0.1, 0.1, 0.1});

    m_light.update();
    m_light.setUniforms(m_shaderHandle);
    m_model.draw();
}

void Lada2110::onImGuiRender() {}

void Lada2110::onDetach() {}
