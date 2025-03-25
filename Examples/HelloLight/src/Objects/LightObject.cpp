#include "LightObject.hpp"

PostprocessingEffect LightObject::m_effect;

void LightObject::setInfoLightObject(const InfoLightObject &infoLightObject) {
    m_infoObject = infoLightObject;
}

void LightObject::onAttach() {
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vstModel3D.glsl", "Shaders/fstModel3D.glsl");
    m_shader = createProgram(programAsset.getBirdProgramCreate());
    m_model.create(m_shader, m_infoObject.pathToModel, m_infoObject.createInfoModel3D);

    //    m_viewport.init();
    m_effect.setPathToShaders("Shaders/vstDebug.glsl", "Shaders/fstDebug.glsl");
    m_effect.onAttach();
}

void LightObject::onUpdate(double deltaTime) {
    auto &transform = m_model.getTransform();
    transform.setPosition(m_infoObject.position);
    transform.setScale(m_infoObject.scale);
    transform.setRotationEuler(m_infoObject.degrees);

    //    m_viewport.update();

    setUniforms();
    //    m_model.setViewId(m_viewport.getViewId());
    m_model.setShader(m_shader);
    m_model.draw();
}

void LightObject::onDetach() {
    m_effect.onDetach();
    deleteProgram(m_shader);
}

void LightObject::onImGuiRender() {
    ImGui::Begin("Camera stats");
    ImGui::Image((ImTextureID)(intptr_t)1, {300, 200});
    ImGui::End();
}

void LightObject::setUniforms() {
    m_light.setUniforms(m_shader);
    m_light.update();
    //    Bird::setShader(m_shader);
    //    int slot = 2;
    //    Bird::setTexture(m_viewport.getTextureHandle(), slot);
    //    Bird::setUniform(m_shader, "shadowMap", &slot, Bird::UniformType::Sampler);
}

void LightObject::drawAll() {
    m_effect.setViewId(0);
    m_effect.onUpdate(0);
}