#pragma once

#include "glm/glm.hpp"
#include "Key.hpp"

class Window {
public:
    ~Window();
    void init(const char* name, uint32_t resolutionX = 1, uint32_t resolutionY = 1, bool fullScreen = true);
    void init(const char* name, bool fullScreen = true);
    bool shouldClose();
    glm::vec2 getCursorPos();
    glm::vec2 getSize();
    double getTime();
    bool isKeyPressed(Key key);
    bool isJustKeyPressed(Key key);
    bool isMouseButtonPressed(MouseButton mouseButton);
    void toggleCursorLock();
    bool isCursorLocked();
    void setShouldClose();
    void swapBuffers();
    void pollEvents();
    void* getNativeHandle();
private:
    void* handle;
    bool cursorLocked;
    uint8_t keys[1024];
};