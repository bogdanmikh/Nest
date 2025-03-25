//
// Created by Bogdan on 24.03.2025.
//

#pragma once

#include "ManagerObjects.hpp"
#include "ManagerLights.hpp"


class LightEnvironment : public Nest::Entity {
public:
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;
private:
    void initObjects();
    ManagerObjects m_managerObjects;
    ManagerLights m_managerLights;

    Bird::ProgramHandle m_shader;
};
