#include "LightObject.hpp"

Nest::Viewport LightObject::m_viewport;

void LightObject::setInfoLightObject(const InfoLightObject &infoLightObject) {
    m_infoObject = infoLightObject;
}

void LightObject::onAttach() {
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vstModel3D.glsl", "Shaders/fstModel3D.glsl");
    m_shaderLight = createProgram(programAsset.getBirdProgramCreate());

    programAsset = Nest::AssetLoader::loadProgram(
        "Shaders/vstDepthModel3D.glsl", "Shaders/fstDepthModel3D.glsl"
    );
    m_shaderDepth = createProgram(programAsset.getBirdProgramCreate());

    setRendererMode(RendererMode::DEPTH);

    m_model.create(*m_currentShader, m_infoObject.pathToModel, m_infoObject.createInfoModel3D);

    m_viewport.init();

    m_effect = F_NEW(Foundation::getAllocator(), PostprocessingEffect);
    m_effect->setPathToShaders("Shaders/vstDebug.glsl", "Shaders/fstDebug.glsl");
    m_effect->setFBTexture(m_viewport.getTextureHandle());
    m_effect->setViewId(0);
    m_effect->onAttach();
}

void LightObject::onUpdate(double deltaTime) {
    auto &transform = m_model.getTransform();
    transform.setPosition(m_infoObject.position);
    transform.setScale(m_infoObject.scale);
    transform.setRotationEuler(m_infoObject.degrees);

    m_viewport.update();
    setRendererMode(RendererMode::DEPTH);
    m_model.setViewId(m_viewport.getViewId());
    m_model.setShader(*m_currentShader);
    m_model.draw();

//    m_effect->onUpdate(deltaTime);

    setRendererMode(RendererMode::DEFAULT);
    setUniforms();
    m_model.setViewId(0);
    m_model.setShader(*m_currentShader);
    m_model.draw();
}

void LightObject::onDetach() {
    m_effect->onDetach();
    F_DELETE(Foundation::getAllocator(), m_effect);
    deleteProgram(m_shaderDepth);
    deleteProgram(m_shaderLight);
}

void LightObject::onImGuiRender() {}

void LightObject::setRendererMode(LightObject::RendererMode rendererMode) {
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

void LightObject::setUniforms() {
    if (m_rendererMode == RendererMode::DEPTH) {
        //            float near_plane = 1.0f, far_plane = 7.5f;
        //            glm::mat4 lightProjection =
        //                glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        //            glm::mat4 lightView = glm::lookAt(
        //                glm::vec3(-2.0f, 4.0f, -1.0f),
        //                glm::vec3(0.0f, 0.0f, 0.0f),
        //                glm::vec3(0.0f, 1.0f, 0.0f)
        //            );
        //
        //            glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        //            Bird::setShader(m_shaderDepth);
        //            Bird::setUniform(
        //                m_shaderDepth, "lightProjViewMtx", &lightSpaceMatrix,
        //                Bird::UniformType::Mat4
        //            );
        return;
    }

    m_light.setUniforms(m_shaderLight);
    m_light.update();

    Bird::setShader(m_shaderLight);
    int slot = 2;
    Bird::setTexture(m_viewport.getTextureHandle(), slot);
    Bird::setUniform(m_shaderDepth, "shadowMap", &slot, Bird::UniformType::Sampler);
}