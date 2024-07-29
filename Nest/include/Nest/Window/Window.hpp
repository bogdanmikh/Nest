#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Window {
public:
    ~Window();
    void init(
        const char *name, uint32_t resolutionX = 1, uint32_t resolutionY = 1, bool fullScreen = true
    );
    void init(const char *name, bool fullScreen = true);
    bool shouldClose();
    void setShouldClose();
    void swapBuffers();
    glm::vec2 getSize();
    double getTime();
    void *getNativeHandle();

private:
    void *handle;
};