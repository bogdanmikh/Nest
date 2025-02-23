//
// Created by Bogdan on 12.11.2024.
//

#include "SimpleLight.hpp"
#include <Bird/Bird.hpp>

SimpleLight::SimpleLight() {
    DirLight dirLight;
    dirLight.direction = {0.2f, 1.0f, -0.3f, 0.f};
    dirLight.ambient = {0.05f, 0.05f, 0.1f, 0.f};
    dirLight.diffuse = {0.2f, 0.2f, 0.7, 0};
    dirLight.specular = {0.7f, 0.7f, 0.7f, 0};
    m_dirLights.emplace_back(dirLight);

    PointLight pointLight;
    pointLight.position = {0, 0, 0, 0};
    pointLight.ambient = {1.0f * 0.1, 0.6f * 0.1, 0.0f, 0};
    pointLight.diffuse = {1.0f, 0.6f, 0.0f, 0};
    pointLight.specular = {1.0f, 0.6f, 0.0f, 0};
    pointLight.constant.r = 1.0;
    pointLight.linear.r = 0.09;
    pointLight.quadratic.r = 0.032;
//    m_pointLights.emplace_back(pointLight);
}

void SimpleLight::setUniforms(Bird::ProgramHandle programHandle) {
    auto camera = Nest::Application::get()->getWorldCamera();

    glm::vec4 numLights;
    numLights.x = m_dirLights.size();
    numLights.y = m_pointLights.size();
    numLights.z = m_spotLights.size();
    Bird::setUniform(
        programHandle, "numLights", &numLights, Bird::UniformType::Vec4
    );
    Bird::setUniform(programHandle, "material.shininess", &m_shininess, Bird::UniformType::Vec4);

    Bird::setUniform(programHandle, "cameraPosVec4", &camera->getPosition()[0], Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, "cameraFrontVec4", &camera->getFront()[0], Bird::UniformType::Vec4);

    for (int i = 0; i < m_dirLights.size(); ++i) {
        setDirLightUniforms(programHandle, m_dirLights[i], i);
    }

    for (int i = 0; i < m_pointLights.size(); ++i) {
        setPointLightUniforms(programHandle, m_pointLights[i], i);
    }

    for (int i = 0; i < m_spotLights.size(); ++i) {
        setSpotLightUniforms(programHandle, m_spotLights[i], i);
    }

}

void SimpleLight::update() {
    auto camera = Nest::Application::get()->getWorldCamera();
    m_pointLights.back().position = {camera->getPosition(), 0};
}


void SimpleLight::setDirLightUniforms(Bird::ProgramHandle programHandle, DirLight &light, int index) {
    std::stringstream beginStr;
    beginStr << "dirLightsVec4[" << index << "].";

    Bird::setUniform(programHandle, (beginStr.str() + "direction").c_str(), &light.direction, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "ambient").c_str(), &light.ambient, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "diffuse").c_str(), &light.diffuse, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "specular").c_str(), &light.specular, Bird::UniformType::Vec4);
}

void SimpleLight::setPointLightUniforms(Bird::ProgramHandle programHandle, SimpleLight::PointLight &light, int index) {
    std::stringstream beginStr;
    beginStr << "pointLightsVec4[" << index << "].";

    Bird::setUniform(programHandle, (beginStr.str() + "position").c_str(), &light.position, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "constant").c_str(), &light.constant, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "linear").c_str(), &light.linear, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "quadratic").c_str(), &light.quadratic, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "ambient").c_str(), &light.ambient, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "diffuse").c_str(), &light.diffuse, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "specular").c_str(), &light.specular, Bird::UniformType::Vec4);
}

void SimpleLight::setSpotLightUniforms(Bird::ProgramHandle programHandle, SimpleLight::SpotLight &light, int index) {
    std::stringstream beginStr;
    beginStr << "spotLightsVec4[" << index << "].";

    Bird::setUniform(programHandle, (beginStr.str() + "position").c_str(), &light.position, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "direction").c_str(), &light.direction, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "cutOff").c_str(), &light.cutOff, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "outerCutOff").c_str(), &light.outerCutOff, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "constant").c_str(), &light.constant, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "linear").c_str(), &light.linear, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "quadratic").c_str(), &light.quadratic, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "ambient").c_str(), &light.ambient, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "diffuse").c_str(), &light.diffuse, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, (beginStr.str() + "specular").c_str(), &light.specular, Bird::UniformType::Vec4);
}