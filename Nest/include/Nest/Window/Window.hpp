#pragma once

#include <glm/glm.hpp>
#include "Nest/Base/Base.hpp"
#include <GLFW/glfw3.h>

namespace Nest {


enum Cursor {
    ARROW = 0,
    IBEAM = 1,
    CROSSHAIR = 2,
    POINTING_HAND = 3,
    RESIZE_EW = 4,
    RESIZE_NS = 5,
    RESIZE_NESW = 6,
    RESIZE_NWSE = 7,
    RESIZE_ALL = 8,
    NOT_ALLOWED = 9,
    COUNT = 10
};


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
    Vec2 getSize();
    double getTime();
    void *getNativeHandle();

    Size getDpi();
    void setCursor(Cursor cursor);
    const char *getClipboardText();
    void setClipboardText(const char *text);
private:
    void *handle;
    GLFWcursor *cursors[Cursor::COUNT];
};

}