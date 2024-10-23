#pragma once

#include <Nest/GameLogic/Entity.hpp>
#include <Nest/Renderer/Model3D.hpp>

class HelloLight final : public Nest::Entity {
public:
    HelloLight();
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;
    Nest::TransformComponent &getTransform();

private:
    Bird::ProgramHandle m_shaderHandle;
    glm::vec4 lightDir;
    Nest::Model3D m_model;
};