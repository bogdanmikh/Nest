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
    AndroidWindow(const char *title, Size size, bool isFullscreen, bool isMaximized);
    ~AndroidWindow() override;
    bool isFullScreen() override;
    void setFullScreen(bool isFullScreen) override;
    void setTitle(const char *title) override;
    bool isMaximized() override;
    void setMaximized(bool isMaximized) override;
    void setResizable(bool isResizable) override;
    void setSize(Size size) override;
    void pollEvents() override;
    void toggleCursorLock() override;
    bool isCursorLocked() override;
    void setCursor(Cursor cursor) override;
    void setEventQueue(EventQueue *eventQueue) override;
    Size getSize() override;
    Size getDpi() override;
    double getTime() override;
    const char *getClipboardText() override;
    void setClipboardText(const char *text) override;

    bool shouldClose() override;
    void setShouldClose() override;

private:
};

} // namespace Nest
