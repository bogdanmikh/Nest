//
// Created by Bogdan on 06.08.2024.
//

#include "CarObject.hpp"
#include "Nest/Assets/AssetLoader.hpp"

#include <fstream>

CarObject::CarObject() {}

void CarObject::onAttach() {
    float lightColor = 0.2;
    m_light.lightColor = glm::vec3(lightColor, lightColor, lightColor);
    m_light.lightPos = glm::vec3(5, 5, 5);
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vstModel3D.glsl", "Shaders/fstModel3D.glsl");
    m_shaderHandle = createProgram(programAsset.getBirdProgramCreate());
    m_model.create(m_shaderHandle, "Models/car/scene.gltf");
}

void CarObject::onUpdate(double deltaTime) {
    auto &transform = m_model.getTransform();
    transform.setPosition({0, 0, 0});
    transform.setScale({0.1, 0.1, 0.1});
    Bird::setUniform(m_shaderHandle, "vec4LightPos", &m_light.lightPos, Bird::UniformType::Vec4);
    Bird::setUniform(
        m_shaderHandle, "vec4LightColor", &m_light.lightColor, Bird::UniformType::Vec4
    );
    m_model.draw();
}

void CarObject::onImGuiRender() {}

void CarObject::onDetach() {}
