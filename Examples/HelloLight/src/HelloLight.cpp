//
// Created by Bogdan on 06.08.2024.
//

#include "HelloLight.hpp"
#include "Nest/Assets/AssetLoader.hpp"

#include <fstream>

HelloLight::HelloLight() {}

void HelloLight::onAttach() {
    m_light.lightColor = glm::vec3(1, 1, 1);
    m_light.lightPos = glm::vec3(5, 5, 5);
    Nest::ProgramAsset programAsset = Nest::AssetLoader::loadProgram(
        "Shaders/Model3d_vertex.glsl", "Shaders/Model3d_fragment.glsl"
    );
    m_shaderHandle = createProgram(programAsset.getBirdProgramCreate());
    m_model.create(m_shaderHandle, "Models/car/scene.gltf");
}

void HelloLight::onUpdate(double deltaTime) {
    auto &transform = m_model.getTransform();
    transform.setPosition({0, 0, 0});
    transform.setScale({0.1, 0.1, 0.1});
    Bird::setUniform(m_shaderHandle, "vec4LightPos", &m_light.lightPos, Bird::UniformType::Vec4);
    Bird::setUniform(
        m_shaderHandle, "vec4LightColor", &m_light.lightColor, Bird::UniformType::Vec4
    );
    m_model.draw();
}

void HelloLight::onImGuiRender() {}

void HelloLight::onDetach() {}
