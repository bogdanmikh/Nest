#pragma once

#include <Nest.hpp>

class CarRenderer final : public Nest::Entity {
public:
    CarRenderer();
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;
    Nest::TransformComponent &getTransform();
private:
    Nest::Model3D m_model3D;
};