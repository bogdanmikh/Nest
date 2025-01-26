#include "GlfwWindow.hpp"
// #include "Nest/GlfwWindow/GlfwEvents.hpp"

#include "Nest/Base/Base.hpp"
#include <Bird/PlatformData.hpp>
#include <Foundation/Logger.hpp>
#include <glad/glad.h>

namespace Nest {

void GlfwWindow::init(
    const char *name, uint32_t resolutionX, uint32_t resolutionY, bool fullScreen
) {
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
    glfwShowWindow((GLFWwindow *)handle);
    glfwFocusWindow((GLFWwindow *)handle);

    Bird::PlatformData::get()->nativeWindowHandle = handle;

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

void GlfwWindow::init(const char *name, bool fullScreen) {
    init(name, 1, 1, fullScreen);
}

GlfwWindow::~GlfwWindow() {
    glfwTerminate();
}

bool GlfwWindow::shouldClose() {
    return glfwWindowShouldClose((GLFWwindow *)handle);
}

Vec2 GlfwWindow::getSize() {
    int x, y;
    float xscale, yscale;
    glfwGetWindowContentScale((GLFWwindow *)handle, &xscale, &yscale);
    glfwGetWindowSize((GLFWwindow *)handle, &x, &y);
    return {x * xscale, y * yscale};
}

double GlfwWindow::getTime() {
    return glfwGetTime();
}

void GlfwWindow::swapBuffers() {
    glfwSwapBuffers((GLFWwindow *)handle);
}

void GlfwWindow::setShouldClose() {
    glfwSetWindowShouldClose((GLFWwindow *)handle, true);
}

void *GlfwWindow::getNativeHandle() {
    return handle;
}

} // namespace Nest