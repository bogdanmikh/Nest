//
// Created by Admin on 19.02.2022.
//

#pragma once

#include <Foundation/CommandBuffer.hpp>

namespace Nest {

enum EventType {
    None = 0,
    WindowResize,
    WindowClose,
    // WindowFocus,
    // WindowLostFocus,
    KeyPressed,
    InputCharacter,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled,
    TouchBegan,
    TouchMoved,
    TouchEnded
};

struct Event : public Foundation::CommandBuffer::Command {
    Event(EventType type)
        : Command(type) {}

    bool isHandled = false;
};

} // namespace Nest
