#pragma once

#include "ObjectSettings.hpp"
#include <Nest/GameLogic/Entity.hpp>
#include <Nest/Renderer/Model3D.hpp>

class SnowScene final : public Nest::Entity {
public:
    SnowScene();
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;
    Nest::TransformComponent &getTransform();
private:
    std::vector<Nest::Model3D> m_objects;
    std::vector<ObjectSettings> m_objectsSettings;
};