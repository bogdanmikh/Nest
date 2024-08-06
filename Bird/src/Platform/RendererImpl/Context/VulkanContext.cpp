//
// Created by Admin on 11.02.2022.
//

#include "VulkanContext.hpp"
#include "Bird/PlatformData.hpp"

#include <Foundation/Logger.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Bird {

void VulkanContext::create() {
    glfwWindowHandle = PlatformData::get()->nativeWindowHandle;
    glfwMakeContextCurrent((GLFWwindow *)glfwWindowHandle);
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false) {
        LOG_CRITICAL("Failed to initialize GLAD");
    }
    // Enable v-sync
    glfwSwapInterval(1);
}

void VulkanContext::flip() {
    glfwSwapBuffers((GLFWwindow *)glfwWindowHandle);
}

uint32_t VulkanContext::getDefaultFrameBufferId() {
    return 0;
}

} // namespace Bird