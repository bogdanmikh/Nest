//
// Created by Bogdan
//

#pragma once

#include "Bird/GraphicsContext.hpp"

namespace Bird {

// GLFW and GLAD based context
class OpenGLContext : public GraphicsContext {
public:
    void create() override;
    void flip() override;
    uint32_t getDefaultFrameBufferId() override;

private:
    void *glfwWindowHandle;
};

} // namespace Bird