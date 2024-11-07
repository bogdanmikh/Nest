//
// Created by Bogdan on 30.10.2024.
//

#include "AndroidContext.hpp"

namespace Bird {

AndroidContext(ANativeWindow *window)
    : nativeWindow(window)
    , display(EGL_NO_DISPLAY)
    , surface(EGL_NO_SURFACE)
    , context(EGL_NO_CONTEXT) {}

~AndroidContext() {
    if (context != EGL_NO_CONTEXT) {
        eglDestroyContext(display, context);
    }
    if (surface != EGL_NO_SURFACE) {
        eglDestroySurface(display, surface);
    }
    if (display != EGL_NO_DISPLAY) {
        eglTerminate(display);
    }
}
void create() {
    // Initialize EGL
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        // Обработка ошибки
        return;
    }

    if (eglInitialize(display, nullptr, nullptr) == EGL_FALSE) {
        // Обработка ошибки
        return;
    }

    // Set the configuration attributes
    EGLint configAttributes[] = {
        EGL_RENDERABLE_TYPE,
        EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE,
        EGL_WINDOW_BIT,
        EGL_RGBA_SIZE,
        8,
        EGL_DEPTH_SIZE,
        16,
        EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    if (eglChooseConfig(display, configAttributes, &config, 1, &numConfigs) == EGL_FALSE ||
        numConfigs == 0) {
        // Обработка ошибки
        return;
    }

    // Create an EGL window surface
    surface = eglCreateWindowSurface(display, config, nativeWindow, nullptr);
    if (surface == EGL_NO_SURFACE) {
        // Обработка ошибки
        return;
    }

    // Create an EGL context
    EGLint contextAttributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttributes);
    if (context == EGL_NO_CONTEXT) {
        // Обработка ошибки
        return;
    }

    // Make the context current
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        // Обработка ошибки
        return;
    }
}

void flip() {
    eglSwapBuffers(display, surface);
}

uint32_t getDefaultFrameBufferId() {
    return 0; // or implement your framebuffer handling.
}

} // namespace Bird