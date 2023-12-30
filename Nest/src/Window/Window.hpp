#pragma once

#include "glm/glm.hpp"
#include "Key.hpp"

class Window {
public:
    Window(const char* name, int resolutionX = 1, int resolutionY = 1, bool fullScreen = true);
    ~Window();
    bool shouldClose();
    glm::vec2 getCursorPos();
    glm::vec2 getSize();
    double getTime();
    bool isKeyPressed(Key key);
    void toggleCursorLock();
    bool isCursorLocked();
    void setShouldClose();
    void swapBuffers();
    void pollEvents();
    void* getNativeHandle();
private:
    void* handle;
    bool cursorLocked;
};