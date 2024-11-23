//
// Created by Admin on 26.01.2022.
//

#pragma once

#include "Nest/Window/Window.hpp"

#include <jni.h>
#include <android/native_activity.h>
#include <android/native_window.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidWindow", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "AndroidWindow", __VA_ARGS__))

namespace Nest {

class AndroidWindow : public Window {
public:
    AndroidWindow();
    ~AndroidWindow() override;
    void init(
        const char *name, uint32_t resolutionX = 1, uint32_t resolutionY = 1, bool fullScreen = true
    ) override;
    void init(const char *name, bool fullScreen = true) override;
    bool shouldClose() override;
    void setShouldClose() override;
    void swapBuffers() override;
    Size getSize() override;
    double getTime() override;
    void *getNativeHandle() override;

private:
    void *handle;
    EGLDisplay eglDisplay;
    EGLSurface eglSurface;
    EGLContext eglContext;
    bool closeRequested;
};

} // namespace Nest
