#pragma once

#include <Nest/GameLogic/Entity.hpp>
#include <Nest/Application/Application.hpp>
#include <Nest/Renderer/Model3D.hpp>

class CarObject final : public Nest::Entity {
public:
    CarObject();
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;
    Nest::TransformComponent &getTransform();

private:
    void updateLight();
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
    Bird::ProgramHandle m_shaderHandle;
    Nest::Model3D m_model;
};