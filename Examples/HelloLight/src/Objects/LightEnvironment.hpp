//
// Created by Bogdan on 24.03.2025.
//

#pragma once

#include "ManagerObjects.hpp"
#include "ManagerLights.hpp"

#include "PostprocessingEffect.hpp"

class LightEnvironment : public Nest::Entity {
public:
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;

private:
    void initObjects();
    void initLights();

    void generateDepthTexture();
    void generateDebugDepthTexture();
    void draw();

    void setDepthUniforms();
    void setSpotLightUniforms(Bird::ProgramHandle programHandle, SpotLight &light, int index);

    ManagerObjects m_managerObjects;
    ManagerLights m_managerLights;

    Bird::ProgramHandle m_depthShader;
    Bird::ProgramHandle m_debugDepthShader;
    Bird::ProgramHandle m_lightShader;

    Nest::Viewport m_depthViewport;
    Nest::Viewport m_debugDepthViewport;

    PostprocessingEffect m_debugDepthEffect;

    glm::mat4 m_lightSpaceMatrix;
};
