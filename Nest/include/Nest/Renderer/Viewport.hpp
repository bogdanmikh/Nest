//
// Created by Bogdan on 29.09.2024.
//

#pragma once

#include "Nest/Base/Base.hpp"
#include <Bird/Bird.hpp>

namespace Nest {

class Viewport final {
public:
    Viewport();
    ~Viewport();

    void init();
    void initWithSize(Vec2 size);
    void update();
    Bird::ViewId getViewId();
    Bird::TextureHandle getTextureHandle();
    Bird::TextureHandle getDepthHandle();

private:
    void updateViewportSize(Nest::Size size);

    static Bird::ViewId m_lastViewId;

    Bird::TextureHandle m_colorAttachment;
    Bird::TextureHandle m_depthAttachment;
    Bird::ViewId m_viewId;
    Nest::Vec2 m_windowSize;
    Bird::FrameBufferSpecification m_fbSpecification;
    Bird::FrameBufferHandle m_frameBuffer;
};

} // namespace Nest