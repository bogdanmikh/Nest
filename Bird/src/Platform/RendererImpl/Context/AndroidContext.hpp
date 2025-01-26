//
// Created by Bogdan on 30.10.2024.
//

#pragma once

#include "Bird/GraphicsContext.hpp"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android/log.h>
#include <android/native_window.h>
#include <stdint.h>

namespace Bird {

class AndroidContext : public GraphicsContext {
public:
    ~AndroidContext() override;
    void create() override;
    void flip() override;
    uint32_t getDefaultFrameBufferId() override;

private:
    EGLDisplay m_display;
    EGLConfig m_config;
    EGLSurface m_surface;
    EGLContext m_context;
};

} // namespace Bird
