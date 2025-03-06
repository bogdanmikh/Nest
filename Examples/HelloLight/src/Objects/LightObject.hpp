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

struct InfoLightObject {
    Nest::Vec3 position = {0, 0, 0};
    Nest::Vec3 scale = {1, 1, 1};
    Nest::Vec3 degrees = {0, 0, 0};

    Nest::Path pathToModel = "";
    Nest::CreateInfoModel3D createInfoModel3D;
};

struct LightObject final : public Nest::Entity {
public:
    void setInfoLightObject(const InfoLightObject &infoLightObject) {
        m_infoObject = infoLightObject;
    }
    void onAttach() override {
        Nest::ProgramAsset programAsset =
            Nest::AssetLoader::loadProgram("Shaders/vstModel3D.glsl", "Shaders/fstModel3D.glsl");
        m_shaderLight = createProgram(programAsset.getBirdProgramCreate());

        programAsset = Nest::AssetLoader::loadProgram("Shaders/vstDepthModel3D.glsl", "Shaders/fstDepthModel3D.glsl");

        m_shaderDepth = createProgram(programAsset.getBirdProgramCreate());
        m_currentShader = &m_shaderLight;

        m_model.create(
            *m_currentShader, m_infoObject.pathToModel, m_infoObject.createInfoModel3D
        );
    }
    void onUpdate(double deltaTime) override {
        setUniforms();

        auto &transform = m_model.getTransform();
        transform.setPosition(m_infoObject.position);
        transform.setScale(m_infoObject.scale);
        transform.setRotationEuler(m_infoObject.degrees);

        m_model.setViewId(0);
        m_model.setShader(*m_currentShader);

        m_model.draw();
    }
    void onImGuiRender() override {}

    void onDetach() override {
        deleteProgram(m_shaderDepth);
        deleteProgram(m_shaderLight);
    }

private:
    enum RendererMode { DEFAULT, DEPTH };

    void setRendererMode(RendererMode rendererMode) {
        m_rendererMode = rendererMode;
        switch (m_rendererMode) {
            case DEFAULT:
                m_currentShader = &m_shaderLight;
                break;
            case DEPTH:
                m_currentShader = &m_shaderDepth;
                break;
        }
    }
    Bird::ProgramHandle m_shaderLight;
    Bird::ProgramHandle m_shaderDepth;
    RendererMode m_rendererMode;
    SimpleLight m_light;
    Nest::Viewport m_viewport;
    void setUniforms() {
        if (m_rendererMode == RendererMode::DEPTH) {
            float near_plane = 1.0f, far_plane = 7.5f;
            glm::mat4 lightProjection =
                glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
            glm::mat4 lightView = glm::lookAt(
                glm::vec3(-2.0f, 4.0f, -1.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );

            glm::mat4 lightSpaceMatrix = lightProjection * lightView;
            Bird::setShader(m_shaderDepth);
            Bird::setUniform(
                m_shaderDepth, "lightProjViewMtx", &lightSpaceMatrix, Bird::UniformType::Mat4
            );
        } else {
            m_light.setUniforms(*m_currentShader);
            m_light.update();
        }
    }

    Bird::ProgramHandle *m_currentShader;
    Bird::TextureHandle m_textureDepth;
    InfoLightObject m_infoObject;

    Nest::Model3D m_model;
};