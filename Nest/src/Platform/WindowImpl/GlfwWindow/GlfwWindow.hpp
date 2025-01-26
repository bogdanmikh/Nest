//
// Created by Admin on 26.01.2022.
//
//
// Created by Admin on 26.01.2022.
//

#pragma once

#include "Nest/Window/Window.hpp"

#include <Foundation/Foundation.hpp>
#include <GLFW/glfw3.h>

namespace Nest {

class GlfwWindow : public Window {
public:
    GlfwWindow(const char *title, Size size, bool isFullscreen, bool isMaximized);
    ~GlfwWindow() override;
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
    void resetCursorPos();
    void addEventHandlers();
    bool m_isCursorLocked;
    bool m_isFullScreen;
    Size m_windowSizeBackup;
    GLFWwindow *m_windowHandle;
    EventQueue *m_eventQueue;
    GLFWcursor *cursors[Cursor::COUNT];
};

} // namespace Nest
