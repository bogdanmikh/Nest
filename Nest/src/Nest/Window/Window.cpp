#include "Nest/Window/Window.hpp"
#include "Nest/Window/Events.hpp"

#include <GLFW/glfw3.h>

#include <Foundation/Logger.hpp>

namespace Nest {

void Window::init(const char *name, uint32_t resolutionX, uint32_t resolutionY, bool fullScreen) {
    if (glfwInit() != GLFW_TRUE) {
        LOG_ERROR("GLFW initialization failed\n");
        return;
    }

    if (fullScreen)
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
#if defined(__APPLE__) || defined(__MACH__)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(resolutionX, resolutionY, name, NULL, NULL);
    if (!window) {
        LOG_ERROR("GLFW window creation failed\n");
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        LOG_CRITICAL("Failed to initialize OpenGL context");
//    }
    this->handle = window;
    Events::init(window);
    glfwShowWindow((GLFWwindow*) handle);
    glfwFocusWindow((GLFWwindow*) handle);
    cursors[Cursor::ARROW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    cursors[Cursor::IBEAM] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    cursors[Cursor::CROSSHAIR] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    cursors[Cursor::POINTING_HAND] = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);
    cursors[Cursor::RESIZE_EW] = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
    cursors[Cursor::RESIZE_NS] = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
#if GLFW_HAS_NEW_CURSORS
    cursors[Cursor::RESIZE_NESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
    cursors[Cursor::RESIZE_NWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
    cursors[Cursor::RESIZE_ALL] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
    cursors[Cursor::NOT_ALLOWED] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
#else
    cursors[Cursor::RESIZE_NESW] = cursors[Cursor::ARROW];
    cursors[Cursor::RESIZE_NWSE] = cursors[Cursor::ARROW];
    cursors[Cursor::RESIZE_ALL] = cursors[Cursor::ARROW];
    cursors[Cursor::NOT_ALLOWED] = cursors[Cursor::ARROW];
#endif

    std::string message = {"\n       |-- \\\n"
                           "       |     \\\n"
                           "       |     /\n"
                           "       |-- /\n"
                           "       |\n"
                           "  \\    |    /\n"
                           "    \\  |  /\n"
                           " α    \\|/    Ω\n"
                           "      /|\\\n"
                           "    /  |  \\\n"
                           "  /    |    \\\n"
                           "   HOC VINCE"};
    LOG_INFO(message);
}

void Window::init(const char *name, bool fullScreen) {
    init(name, 1, 1, fullScreen);
}

Window::~Window() {
    glfwTerminate();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose((GLFWwindow *)handle);
}

Vec2 Window::getSize() {
    int x, y;
    float xscale, yscale;
    glfwGetWindowContentScale((GLFWwindow *)handle, &xscale, &yscale);
    glfwGetWindowSize((GLFWwindow *)handle, &x, &y);
    return {x * xscale, y * yscale};
}

Size Window::getDpi() {
    float xscale, yscale;
    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);
    return Size(xscale, yscale);
}

double Window::getTime() {
    return glfwGetTime();
}

void Window::swapBuffers() {
    glfwSwapBuffers((GLFWwindow *)handle);
}

void Window::setShouldClose() {
    glfwSetWindowShouldClose((GLFWwindow *)handle, true);
}

void *Window::getNativeHandle() {
    return handle;
}

const char *Window::getClipboardText() {
    return glfwGetClipboardString((GLFWwindow *)handle);
}

void Window::setClipboardText(const char *text) {
    glfwSetClipboardString((GLFWwindow*) handle, text);
}

void Window::setCursor(Cursor cursor) {
    GLFWcursor *glfwCursor = cursors[cursor];
    glfwSetCursor((GLFWwindow*) handle, glfwCursor);
}

}