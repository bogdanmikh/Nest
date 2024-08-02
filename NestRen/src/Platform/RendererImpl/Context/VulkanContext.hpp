//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "NestRen/GraphicsContext.hpp"

namespace NestRen {

// GLFW and GLAD based context
class VulkanContext : public GraphicsContext {
public:
    void create() override;
    void flip() override;
    uint32_t getDefaultFrameBufferId() override;

private:
    void *glfwWindowHandle;
};

} // namespace NestRen
