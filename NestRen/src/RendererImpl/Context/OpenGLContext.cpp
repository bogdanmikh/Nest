//
// Created by Admin on 11.02.2022.
//

#include "OpenGLContext.hpp"
#include "Miren/PlatformData.hpp"

#include <Foundation/Logger.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Miren {

void OpenGLContext::create() {
    glfwWindowHandle = PlatformData::get()->nativeWindowHandle;
    glfwMakeContextCurrent((GLFWwindow *)glfwWindowHandle);
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false) {
        LOG_CRITICAL("Failed to initialize GLAD");
    }
    // Enable v-sync
    glfwSwapInterval(1);
}

void OpenGLContext::flip() {
    glfwSwapBuffers((GLFWwindow *)glfwWindowHandle);
}

uint32_t OpenGLContext::getDefaultFrameBufferId() {
    return 0;
}

} // namespace Miren