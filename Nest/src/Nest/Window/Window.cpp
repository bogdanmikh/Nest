#include <iostream>

#include <glad/glad.h>
#include <cstring>

#include "Nest/Window/Window.hpp"
#include "Nest/Window/Events.hpp"
#include "Nest/Logger/Logger.hpp"

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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to initialize OpenGL context");
    }
    this->handle = window;
    Events::init(window);

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

glm::vec2 Window::getSize() {
    int x, y;
    float xscale, yscale;
    glfwGetWindowContentScale((GLFWwindow *)handle, &xscale, &yscale);
    glfwGetWindowSize((GLFWwindow *)handle, &x, &y);
    return {x * xscale, y * yscale};
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
