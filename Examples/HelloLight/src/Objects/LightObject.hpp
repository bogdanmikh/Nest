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

    static void drawAll();

    // сначала глубину всех 3д моделей
    // свет и цвет потом
private:
    void setUniforms();
    Bird::ProgramHandle m_shader;

    Bird::TextureHandle m_textureDepth;

    Nest::Viewport m_result;
    //    static Nest::Viewport m_viewport;

    Nest::Model3D m_model;

    InfoLightObject m_infoObject;

    SimpleLight m_light;

    static PostprocessingEffect m_effect;
};