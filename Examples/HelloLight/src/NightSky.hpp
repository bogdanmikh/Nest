//
// Created by Bogdan on 23.10.2024.
//

#pragma once

#include <Nest.hpp>

class NightSky : public Nest::Entity {
public:
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;
    Nest::TransformComponent &getTransform();

private:
    Nest::SkyComponent *m_nightSky;
    glm::mat4 projViewMtx;
};
