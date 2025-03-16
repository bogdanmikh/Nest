//
// Created by Bogdan on 06.03.2025.
//

#pragma once

#include <Nest/GameLogic/Entity.hpp>
#include <Nest/Assets/AssetLoader.hpp>
#include <Bird/Base.hpp>
#include <Bird/Bird.hpp>
#include "SimpleLight.hpp"
#include "Nest/Renderer/Viewport.hpp"
#include "PostprocessingEffect.hpp"

struct InfoLightObject {
    Nest::Vec3 position = {0, 0, 0};
    Nest::Vec3 scale = {1, 1, 1};
    Nest::Vec3 degrees = {0, 0, 0};

    Nest::Path pathToModel = "";
    Nest::CreateInfoModel3D createInfoModel3D;
};

struct LightObject final : public Nest::Entity {
public:
    void setInfoLightObject(const InfoLightObject &infoLightObject);
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;

    void onDetach() override;

private:
    enum RendererMode { DEFAULT, DEPTH };

    void setRendererMode(RendererMode rendererMode);

    void setUniforms();

    Bird::ProgramHandle *m_currentShader;
    Bird::ProgramHandle m_shaderDepth;
    Bird::ProgramHandle m_shaderLight;

    Bird::TextureHandle m_textureDepth;

    static Nest::Viewport m_viewport;
    Nest::Model3D m_model;

    InfoLightObject m_infoObject;
    RendererMode m_rendererMode;

    SimpleLight m_light;
    
    PostprocessingEffect *m_effect;
};