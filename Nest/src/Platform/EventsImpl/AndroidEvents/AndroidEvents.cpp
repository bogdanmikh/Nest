#include "AndroidEvents.hpp"

#include <cstring>

namespace Nest {

void AndroidEvents::init(void *handle) {}

glm::vec2 AndroidEvents::getCursorPos() {
    return {0, 0};
}

double AndroidEvents::getTime() {
    return time(nullptr);
}

bool AndroidEvents::isKeyPressed(Key key) {
    return false;
}

bool AndroidEvents::isJustKeyPressed(Key key) {
    return false;
}

bool AndroidEvents::isMouseButtonPressed(MouseButton mouseButton) {
    return false;
}

bool AndroidEvents::isJustMouseButtonPressed(MouseButton mouseButton) {
    return false;
}

void AndroidEvents::pollEvents() {}

void AndroidEvents::toggleCursorLock() {}
bool AndroidEvents::isCursorLocked() {
    return false;
}

std::vector<std::string> AndroidEvents::getDropPaths() {
    return {};
}

void AndroidEvents::resetDropPaths() {}

} // namespace Nest