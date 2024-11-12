#pragma once

#include <Nest/GameLogic/Entity.hpp>
// #include <Nest/Renderer/NaniteModel3D.hpp>
#include <Nest/Renderer/Model3D.hpp>

#include "SimpleLight.hpp"

class StreetLamp final : public Nest::Entity {
public:
    StreetLamp();
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