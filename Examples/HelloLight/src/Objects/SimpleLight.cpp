//
// Created by Bogdan on 12.11.2024.
//

#include "SimpleLight.hpp"
#include <Bird/Bird.hpp>

SimpleLight::SimpleLight() {
    m_shininess.x = 32;
    DirLight dirLight;
    dirLight.direction = {-0.2f, -1.0f, -0.3f, 0.f};
    dirLight.ambient = {0.0f, 0.0f, 0.0f, 0.f};
    dirLight.diffuse = {0.05f, 0.05f, 0.05, 0};
    dirLight.specular = {0.2f, 0.2f, 0.2, 0};
    m_dirLights.emplace_back(dirLight);

    // d - distance light to camera
    // intensity = 1 / constant + linear * d + quadratic * d * d
    PointLight pointLight;
    pointLight.position = {13.3, 7.7, 0.9, 0};
    pointLight.ambient = {0.0f, 0.0f, 1.0f, 0};
    pointLight.diffuse = {1.0f, 0.6f, 0.0f, 0};
    pointLight.specular = {1.0f, 0.6f, 0.0f, 0};
    pointLight.constant.r = 1.0;
    pointLight.linear.r = 0.09;
    pointLight.quadratic.r = 0.032;
    m_pointLights.emplace_back(pointLight);

    pointLight.position = {16.5, 7.7, 0.9, 0};
    pointLight.ambient = {1.0f, 0.0f, 0.0f, 0};
    pointLight.diffuse = {1.0f, 0.6f, 0.0f, 0};
    pointLight.specular = {1.0f, 0.6f, 0.0f, 0};
    pointLight.constant.r = 0.5;
    pointLight.linear.r = 0.09;
    pointLight.quadratic.r = 0.032;
    m_pointLights.emplace_back(pointLight);

    SpotLight spotLight;
    spotLight.position = {-3.4, 21, 2.8, 0};
    spotLight.direction = {0.18, -0.87, -0.45, 0};
    spotLight.ambient = {1, 1., 1., 0};
    spotLight.diffuse = {1.0f, 1.0f, 1.0, 0};
    spotLight.specular = {1.0f, 1.0f, 1.0, 0};
    spotLight.constant.r = 0.4f;
    spotLight.linear.r = 0.009f;
    spotLight.quadratic.r = 0.0032f;
    spotLight.cutOff.r = glm::cos(glm::radians(2.0f));
    spotLight.outerCutOff.r = glm::cos(glm::radians(20.5f));
    m_spotLights.emplace_back(spotLight);
}

void SimpleLight::setUniforms(Bird::ProgramHandle programHandle) {
    auto camera = Nest::Application::get()->getWorldCamera();

    auto &spot = m_spotLights.back();

    spot.position.x = camera->getPosition().x;
    spot.position.y = camera->getPosition().y;
    spot.position.z = camera->getPosition().z;

    spot.direction.x = camera->getFront().x;
    spot.direction.y = camera->getFront().y;
    spot.direction.z = camera->getFront().z;

    glm::vec4 numLights;
    numLights.x = m_dirLights.size();
    numLights.y = m_pointLights.size();
    numLights.z = m_spotLights.size();
    Bird::setUniform(programHandle, "numLights", &numLights, Bird::UniformType::Vec4);
    Bird::setUniform(programHandle, "material.shininess", &m_shininess, Bird::UniformType::Vec4);

    Bird::setUniform(
        programHandle, "cameraPosVec4", &camera->getPosition()[0], Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle, "cameraFrontVec4", &camera->getFront()[0], Bird::UniformType::Vec4
    );

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
    auto time = Nest::Application::get()->getWindow()->getTime();
    float diff = (time - int(time)) * 10;
    int random = Nest::getRandomInt(0, 10);
    if (diff - 3 >= random) {
        m_spotLights.back().ambient = {0, 0, 0, 0};
    } else {
        m_spotLights.back().ambient = {1, 0.8, 0.23, 0};
    }
    //    LOG_INFO("{}", time);
    if (!m_spotLights.empty()) {
        //        m_spotLights.back().position = {camera->getPosition(), 0};
        //        m_spotLights.back().direction = {camera->getFront(), 0};
        //        LOG_INFO(
        //            "{} {} {}", camera->getPosition().x, camera->getPosition().y,
        //            camera->getPosition().z
        //        );
        //        LOG_INFO("{} {} {}", camera->getFront().x, camera->getFront().y,
        //        camera->getFront().z);
    }
}

void SimpleLight::setDirLightUniforms(
    Bird::ProgramHandle programHandle, DirLight &light, int index
) {
    std::stringstream beginStr;
    beginStr << "dirLightsVec4[" << index << "].";

    Bird::setUniform(
        programHandle,
        (beginStr.str() + "direction").c_str(),
        &light.direction,
        Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle, (beginStr.str() + "ambient").c_str(), &light.ambient, Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle, (beginStr.str() + "diffuse").c_str(), &light.diffuse, Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle,
        (beginStr.str() + "specular").c_str(),
        &light.specular,
        Bird::UniformType::Vec4
    );
}

void SimpleLight::setPointLightUniforms(
    Bird::ProgramHandle programHandle, SimpleLight::PointLight &light, int index
) {
    std::stringstream beginStr;
    beginStr << "pointLightsVec4[" << index << "].";

    Bird::setUniform(
        programHandle,
        (beginStr.str() + "position").c_str(),
        &light.position,
        Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle,
        (beginStr.str() + "constant").c_str(),
        &light.constant,
        Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle, (beginStr.str() + "linear").c_str(), &light.linear, Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle,
        (beginStr.str() + "quadratic").c_str(),
        &light.quadratic,
        Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle, (beginStr.str() + "ambient").c_str(), &light.ambient, Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle, (beginStr.str() + "diffuse").c_str(), &light.diffuse, Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle,
        (beginStr.str() + "specular").c_str(),
        &light.specular,
        Bird::UniformType::Vec4
    );
}

void SimpleLight::setSpotLightUniforms(
    Bird::ProgramHandle programHandle, SimpleLight::SpotLight &light, int index
) {
    std::stringstream beginStr;
    beginStr << "spotLightsVec4[" << index << "].";

    Bird::setUniform(
        programHandle,
        (beginStr.str() + "position").c_str(),
        &light.position,
        Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle,
        (beginStr.str() + "direction").c_str(),
        &light.direction,
        Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle, (beginStr.str() + "cutOff").c_str(), &light.cutOff, Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle,
        (beginStr.str() + "outerCutOff").c_str(),
        &light.outerCutOff,
        Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle,
        (beginStr.str() + "constant").c_str(),
        &light.constant,
        Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle, (beginStr.str() + "linear").c_str(), &light.linear, Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle,
        (beginStr.str() + "quadratic").c_str(),
        &light.quadratic,
        Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle, (beginStr.str() + "ambient").c_str(), &light.ambient, Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle, (beginStr.str() + "diffuse").c_str(), &light.diffuse, Bird::UniformType::Vec4
    );
    Bird::setUniform(
        programHandle,
        (beginStr.str() + "specular").c_str(),
        &light.specular,
        Bird::UniformType::Vec4
    );
}