//
// Created by Bogdan on 06.08.2024.
//

#include "HelloLight.hpp"
#include "Nest/Assets/AssetLoader.hpp"

#include <fstream>

HelloLight::HelloLight() {}

void HelloLight::onAttach() {
    Nest::ProgramAsset programAsset = Nest::AssetLoader::loadProgram(
        "Shaders/Model3d_vertex.glsl", "Shaders/Model3d_fragment.glsl"
    );
    m_shaderHandle = createProgram(programAsset.getBirdProgramCreate());
    m_model.create(m_shaderHandle, "Models/car/scene.gltf");
}

void HelloLight::onUpdate(double deltaTime) {
    lightDir = {-0.2, -1.0, -0.3, 0};
    auto &transform = m_model.getTransform();
    transform.setPosition({0, 0, 0});
    transform.setScale({0.1, 0.1, 0.1});
    Bird::setUniform(m_shaderHandle, "directionLight", &lightDir, Bird::UniformType::Vec4);
    m_model.draw();
}

void HelloLight::onImGuiRender() {}

void HelloLight::onDetach() {}
