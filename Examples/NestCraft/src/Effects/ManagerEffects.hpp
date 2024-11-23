//
// Created by Bogdan on 30.09.2024.
//

#pragma once

#include <Nest.hpp>

class PostprocessingEffect;

class ManagerEffects : public Nest::Entity {
public:
    ManagerEffects();
    void setFBTexture(Bird::TextureHandle textureHandle);
    void setViewId(Bird::ViewId viewId);
    void onAttach() override;
    void onDetach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;

private:
    static const int countEffects = 5;
    std::array<bool, countEffects> m_activeEffects;
    Bird::TextureHandle m_textureHandle;
    std::array<PostprocessingEffect *, countEffects> m_effects;
    Bird::ViewId m_viewId;
};
