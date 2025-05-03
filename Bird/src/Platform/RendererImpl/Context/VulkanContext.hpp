//
// Created by Bogdan on 06.04.2025.
//

#pragma once

#include "Bird/GraphicsContext.hpp"
#include <GLFW/glfw3.h>

namespace Bird {

// GLFW and GLAD based context
class VulkanContext : public GraphicsContext {
public:
    void create() override;
    void flip() override;
    uint32_t getDefaultFrameBufferId() override;

private:
    void *glfwWindowHandle;
};

} // namespace Bird