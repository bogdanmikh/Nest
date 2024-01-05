#include "Nest/Window/Window.hpp"

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

Window::Window(const char* name, int resolutionX, int resolutionY, bool fullScreen)
: cursorLocked(false) {
    if (glfwInit() != GLFW_TRUE) {
        std::cout << "GLFW initialization failed\n";
        return;
    }

    if (fullScreen) glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
#if defined(__APPLE__) || defined(__MACH__)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(resolutionX, resolutionY, name, NULL, NULL);
    if (!window) {
        std::cout << "GLFW window creation failed\n";
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
    }
    this->handle = window;
}

Window::~Window() {
    glfwTerminate();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose((GLFWwindow*) handle);
}

glm::vec2 Window::getCursorPos() {
    double x, y;
    glfwGetCursorPos((GLFWwindow*) handle, &x, &y);
    return { x, y };
}

glm::vec2 Window::getSize() {
    int x, y;
    float xscale, yscale;
    glfwGetWindowContentScale((GLFWwindow*) handle, &xscale, &yscale);
    glfwGetWindowSize((GLFWwindow*) handle,  &x, &y);
    return { x * xscale, y * yscale };
}

double Window::getTime() {
    return glfwGetTime();
}

bool Window::isKeyPressed(Key key) {
    return glfwGetKey((GLFWwindow*) handle, (int) key) == GLFW_PRESS;
}

void Window::setShouldClose() {
    glfwSetWindowShouldClose((GLFWwindow*) handle, true);
}

void Window::swapBuffers() {
    glfwSwapBuffers((GLFWwindow*) handle);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::toggleCursorLock() {
    cursorLocked = cursorLocked == false;
    glfwSetInputMode((GLFWwindow*) handle, GLFW_CURSOR, cursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void* Window::getNativeHandle() {
    return handle;
}

bool Window::isCursorLocked() {
    return cursorLocked;
}
