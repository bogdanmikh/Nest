//
// Created by Bogdan
//

#pragma once

#include "Platform/RendererImpl/OpenGL/OpenGLFrameBuffer.hpp"

namespace Bird {
class GraphicsContext {
public:
    virtual ~GraphicsContext() = default;
    virtual void create() = 0;
    virtual void flip() = 0;
    virtual uint32_t getDefaultFrameBufferId() = 0;
};
} // namespace Bird