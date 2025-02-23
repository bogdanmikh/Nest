#pragma once

#include <Nest/Application/Application.hpp>
#include <Nest/GameLogic/Entity.hpp>
#include <Nest/Renderer/Model3D.hpp>

#include "SimpleLight.hpp"

class DpsLada final : public Nest::Entity {
public:
    DpsLada();
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;
    Nest::TransformComponent &getTransform();

private:
    SimpleLight m_light;
    Bird::ProgramHandle m_shaderHandle;
    Nest::Model3D m_model;
};