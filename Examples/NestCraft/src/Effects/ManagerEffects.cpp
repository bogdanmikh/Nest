//
// Created by Bogdan on 30.09.2024.
//

#include "ManagerEffects.hpp"
#include "PostprocessingEffect.hpp"

void ManagerEffects::setFBTexture(Bird::TextureHandle textureHandle) {
    m_textureHandle = textureHandle;
}

void ManagerEffects::setViewId(Bird::ViewId viewId) {
    m_viewId = viewId;
}

ManagerEffects::ManagerEffects()
    : m_viewId(0) {}

void ManagerEffects::onAttach() {
    if (!m_textureHandle.isValid()) {
        LOG_CRITICAL("Texture Handle is NOT VALID");
    }
    m_activeEffects[1] = false;
    m_activeEffects[2] = false;
    m_activeEffects[3] = false;
    m_activeEffects[4] = false;

    // default
//    m_effects[0] = F_NEW(Foundation::getAllocator(), PostprocessingEffect);
//    m_effects[0]->setPathToShaders("Shaders/vstEffects.glsl", "Shaders/fstDefault.glsl");
//    m_effects[0]->setFBTexture(m_textureHandle);
//    m_effects[0]->setViewId(0);
//    m_effects[0]->onAttach();
#ifdef PLATFORM_DESKTOP
    // pixels
    m_effects[1] = F_NEW(Foundation::getAllocator(), PostprocessingEffect);
    m_effects[1]->setPathToShaders("Shaders/vstEffects.glsl", "Shaders/fstPixel.glsl");
    m_effects[1]->setFBTexture(m_textureHandle);
    m_effects[1]->setViewId(m_viewId);
    m_effects[1]->onAttach();
    // wave
    m_effects[2] = F_NEW(Foundation::getAllocator(), PostprocessingEffect);
    m_effects[2]->setPathToShaders("Shaders/vstEffects.glsl", "Shaders/fstWave.glsl");
    m_effects[2]->setFBTexture(m_textureHandle);
    m_effects[2]->setViewId(m_viewId);
    m_effects[2]->onAttach();
    // tv
    m_effects[3] = F_NEW(Foundation::getAllocator(), PostprocessingEffect);
    m_effects[3]->setPathToShaders("Shaders/vstEffects.glsl", "Shaders/fstTv.glsl");
    m_effects[3]->setFBTexture(m_textureHandle);
    m_effects[3]->setViewId(m_viewId);
    m_effects[3]->onAttach();
    // Lens Dispersion
    m_effects[4] = F_NEW(Foundation::getAllocator(), PostprocessingEffect);
    m_effects[4]->setPathToShaders("Shaders/vstEffects.glsl", "Shaders/fstLensDispersion.glsl");
    m_effects[4]->setFBTexture(m_textureHandle);
    m_effects[4]->setViewId(m_viewId);
    m_effects[4]->onAttach();
#endif
}

void ManagerEffects::onUpdate(double deltaTime) {
    for (int i = 1; i < countEffects; ++i) {
        if (m_activeEffects[i]) {
            m_effects[i]->onUpdate(deltaTime);
            break;
        }
    }
//    m_effects[0]->onUpdate(deltaTime);
}

void ManagerEffects::onImGuiRender() {
#ifndef PLATFORM_ANDROID
    ImGuiIO io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 100, 0));
    ImGui::Begin("Effects");
    ImGui::Checkbox("Pixel", &m_activeEffects[1]);
    ImGui::Checkbox("Wave", &m_activeEffects[2]);
    ImGui::Checkbox("Tv", &m_activeEffects[3]);
    ImGui::Checkbox("Lens", &m_activeEffects[4]);
    ImGui::End();
#endif
}

void ManagerEffects::onDetach() {
    for (int i = 0; i < m_effects.size(); ++i) {
        m_effects[i]->onDetach();
        F_DELETE(Foundation::getAllocator(), m_effects[i]);
    }
}