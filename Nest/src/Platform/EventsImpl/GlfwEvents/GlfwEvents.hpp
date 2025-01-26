#pragma once

#include <GLFW/glfw3.h>
#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Nest/Window/Events.hpp"

namespace Nest {

class GlfwEvents final : public Events {
public:
    void init(void *handle) override;
    glm::vec2 getCursorPos() override;
    double getTime() override;
    bool isKeyPressed(Key key) override;
    bool isJustKeyPressed(Key key) override;
    bool isMouseButtonPressed(MouseButton mouseButton) override;
    bool isJustMouseButtonPressed(MouseButton mouseButton) override;
    void toggleCursorLock() override;
    bool isCursorLocked() override;
    void pollEvents() override;
    std::vector<std::string> getDropPaths() override;
    void resetDropPaths() override;

private:
    static void *m_handle;
    static bool cursorLocked;
    static bool keys[1024];
    static uint32_t framesKeys[1024];
    static bool mouseButtons[8];
    static uint32_t framesMouseButtons[8];
    static std::vector<std::string> m_dropPaths;
    static uint32_t frame;
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow *window, int button, int action, int mode);
    static void dropCallback(GLFWwindow *window, int count, const char **paths);
};

} // namespace Nest