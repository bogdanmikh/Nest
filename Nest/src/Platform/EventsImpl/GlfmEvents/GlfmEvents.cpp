#include "GlfmEvents.hpp"

#include <glfm.h>

namespace Nest {

void GlfmEvents::init(void *handle) {}

glm::vec2 GlfmEvents::getCursorPos() {
    return {0, 0};
}

double GlfmEvents::getTime() {
    return 0;
}

bool GlfmEvents::isKeyPressed(Key key) {
    return false;
}

bool GlfmEvents::isJustKeyPressed(Key key) {
    return false;
}

bool GlfmEvents::isMouseButtonPressed(MouseButton mouseButton) {
    return false;
}

bool GlfmEvents::isJustMouseButtonPressed(MouseButton mouseButton) {
    return false;
}

void GlfmEvents::pollEvents() {}

void GlfmEvents::toggleCursorLock() {}
bool GlfmEvents::isCursorLocked() {
    return false;
}

std::vector<std::string> GlfmEvents::getDropPaths() {
    return {};
}

void GlfmEvents::resetDropPaths() {}

} // namespace Nest