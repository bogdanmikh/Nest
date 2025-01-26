//
// Created by Admin on 26.01.2022.
//

#pragma once

#include "Nest/Window/Window.hpp"

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/log.h>
#include <android/native_activity.h>
#include <android/native_window.h>
#include <jni.h>

namespace Nest {

class AndroidWindow : public Window {
public:
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
};

} // namespace Nest
