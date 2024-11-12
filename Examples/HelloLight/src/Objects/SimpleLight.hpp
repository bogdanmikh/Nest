//
// Created by Bogdan on 12.11.2024.
//

#pragma once

#include <Bird/Base.hpp>
#include <Nest/Application/Application.hpp>

class SimpleLight {
public:
    void update();
    void setUniforms(Bird::ProgramHandle programHandle);

private:
    struct Light {
        glm::vec4 position;
        glm::vec4 direction;
        glm::vec4 cutOff;
        glm::vec4 outerCutOff;

        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;

        glm::vec4 constant;
        glm::vec4 linear;
        glm::vec4 quadratic;
    };

    glm::vec4 m_shininess;
    Light m_light;
};
