#pragma once

#include <Nest/GameLogic/Entity.hpp>
// #include <Nest/Renderer/NaniteModel3D.hpp>
#include <Nest/Renderer/Model3D.hpp>

class StreetLamp final : public Nest::Entity {
public:
    StreetLamp();
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;
    Nest::TransformComponent &getTransform();

private:
    struct Light {
        glm::vec3 lightPos;
        glm::vec3 lightColor;
    };
    Light m_light;
    Bird::ProgramHandle m_shaderHandle;
    Nest::Model3D m_model;
};