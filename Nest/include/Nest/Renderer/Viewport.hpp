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
    void update();
    Bird::ViewId getViewId();
    Bird::TextureHandle getTextureHandle();

private:
    void updateViewportSize(Nest::Size size);

    Bird::TextureHandle m_colorAttachment;
    Bird::ViewId m_viewId;
    Nest::Vec2 m_windowSize;
    Bird::FrameBufferSpecification m_fbSpecification;
    Bird::FrameBufferHandle m_frameBuffer;
};

} // namespace Nest