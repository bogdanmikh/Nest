#include "Nest/Window/Events.hpp"

#include "Nest/Application/Application.hpp"
#include <cstring>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

bool Events::keys[];
bool Events::mouseButtons[];
uint32_t Events::framesKeys[];
uint32_t Events::framesMouseButtons[];
uint32_t Events::frame = 0;
bool Events::cursorLocked = false;
void* Events::m_handle = nullptr;

void Events::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
        framesKeys[key] = frame;
    } else if (action == GLFW_RELEASE){
        keys[key] = false;
        framesKeys[key] = frame;
    }
}
void Events::mouseCallback(GLFWwindow* window, int button, int action, int mode) {
    if (action == GLFW_PRESS){
        keys[button] = true;
        framesMouseButtons[button] = frame;
    } else if (action == GLFW_RELEASE){
        keys[button] = false;
        framesMouseButtons[button] = frame;
    }
}

void Events::init(void* handle) {
    frame = 0;
    cursorLocked = false;
    m_handle = handle;

    memset(keys, false, 1024 * sizeof(bool));
    memset(mouseButtons, false, 8 * sizeof(bool));
    memset(framesKeys, 0, 1024 * sizeof(uint32_t));
    memset(framesMouseButtons, 0, 8 * sizeof(uint32_t));

    glfwSetKeyCallback((GLFWwindow *) m_handle, keyCallback);
    glfwSetMouseButtonCallback((GLFWwindow *) m_handle, mouseCallback);
}

glm::vec2 Events::getCursorPos() {
    double x, y;
    glfwGetCursorPos((GLFWwindow *) m_handle, &x, &y);
    return { x, y };
}

double Events::getTime() {
    return glfwGetTime();
}

bool Events::isKeyPressed(Key key) {
    if (int(key) < 0 || int(key) >= 1024) {
        return false;
    }
    return keys[uint32_t(key)];
}

bool Events::isJustKeyPressed(Key key) {
    if (int(key) < 0 || int(key) >= 1024) {
        return false;
    }
    return keys[uint32_t(key)] && framesKeys[int(key)] == frame;
}

bool Events::isMouseButtonPressed(MouseButton mouseButton) {
    if (int(mouseButton) < 0 || int(mouseButton) >= 8) {
        return false;
    }
    return keys[int(mouseButton)];
}

bool Events::isJustMouseButtonPressed(MouseButton mouseButton) {
    if (int(mouseButton) < 0 || int(mouseButton) >= 8) {
        return false;
    }
    return keys[int(mouseButton)] && framesMouseButtons[int(mouseButton)] == frame;
}

void Events::pollEvents() {
    frame++;
    glfwPollEvents();
}

void Events::toggleCursorLock() {
    cursorLocked = !cursorLocked;
    glm::vec2 cursorPos = getCursorPos();

    glfwSetCursorPos((GLFWwindow *) m_handle, cursorPos.x, cursorPos.y);
    glfwSetInputMode((GLFWwindow *) m_handle, GLFW_CURSOR, cursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

bool Events::isCursorLocked() {
    return cursorLocked;
}

