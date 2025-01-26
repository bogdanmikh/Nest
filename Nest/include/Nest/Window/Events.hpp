//
// Created by Bogdan on 28.10.2024.
//

#pragma once

#include "Key.hpp"
#include <glm/vec2.hpp>

namespace Nest {

class Events {
public:
    virtual ~Events() = default;
    virtual void init(void *handle) = 0;
    virtual glm::vec2 getCursorPos() = 0;
    virtual double getTime() = 0;
    virtual bool isKeyPressed(Key key) = 0;
    virtual bool isJustKeyPressed(Key key) = 0;
    virtual bool isMouseButtonPressed(MouseButton mouseButton) = 0;
    virtual bool isJustMouseButtonPressed(MouseButton mouseButton) = 0;
    virtual void toggleCursorLock() = 0;
    virtual bool isCursorLocked() = 0;
    virtual void pollEvents() = 0;
    virtual std::vector<std::string> getDropPaths() = 0;
    virtual void resetDropPaths() = 0;
};

} // namespace Nest