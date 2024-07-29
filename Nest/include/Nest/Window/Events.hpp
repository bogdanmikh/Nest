#pragma once

#include <cstdint>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "Nest/Window/Key.hpp"

class Events final {
public:
    static void init(void *handle);
    static glm::vec2 getCursorPos();
    static double getTime();
    static bool isKeyPressed(Key key);
    static bool isJustKeyPressed(Key key);
    static bool isMouseButtonPressed(MouseButton mouseButton);
    static bool isJustMouseButtonPressed(MouseButton mouseButton);
    static void toggleCursorLock();
    static bool isCursorLocked();
    static void pollEvents();
    static std::vector<std::string> getDropPaths();
    static void resetDropPaths();

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