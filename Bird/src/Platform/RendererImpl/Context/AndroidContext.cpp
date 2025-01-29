//
// Created by Bogdan on 30.10.2024.
//

#include "AndroidContext.hpp"

#include "AndroidContext.hpp"
#include "Bird/Bird.hpp"
#include "Bird/PlatformData.hpp"
#include <cassert>

namespace Bird {

AndroidContext::~AndroidContext() {
    eglDestroyContext(m_display, m_context);
    eglDestroySurface(m_display, m_surface);
    eglTerminate(m_display);
}

void AndroidContext::create() {
    m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(m_display);

    auto res = eglInitialize(m_display, nullptr, nullptr);
    assert(res == EGL_TRUE);

    // clang-format off
    constexpr EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RED_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_NONE
    };
    // clang-format on
    EGLint numConfigs;
    eglChooseConfig(m_display, attribs, &m_config, 1, &numConfigs);
    assert(numConfigs == 1);

    m_surface = eglCreateWindowSurface(
        m_display,
        m_config,
        (EGLNativeWindowType)Bird::PlatformData::get()->nativeWindowHandle,
        nullptr
    );
    assert(m_surface != EGL_NO_SURFACE);

    // Create a GLES 3 context
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};

    m_context = eglCreateContext(m_display, m_config, nullptr, contextAttribs);

    assert(m_context != EGL_NO_CONTEXT);

    res = eglMakeCurrent(m_display, m_surface, m_surface, m_context);
}

void AndroidContext::flip() {
    auto res = eglSwapBuffers(m_display, m_surface);
    assert(res);
}

uint32_t AndroidContext::getDefaultFrameBufferId() {
    return 0; // or implement your framebuffer handling.
}

} // namespace Bird