//
// Created by Admin on 19.02.2022.
//

#pragma once

#include "Nest/Events/Event.hpp"

namespace Nest {

struct MouseMovedEvent : public Event {
    MouseMovedEvent(double x, double y)
        : Event(EventType::MouseMoved)
        , x(x)
        , y(y) {}

    double x, y;
};

struct MouseScrolledEvent : public Event {
    MouseScrolledEvent(double xoffset, double yoffset)
        : Event(EventType::MouseScrolled)
        , xoffset(xoffset)
        , yoffset(yoffset) {}

    double xoffset;
    double yoffset;
};

struct MouseKeyEvent : public Event {
    MouseKeyEvent(MouseButton button, bool isPressed)
        : Event(isPressed ? EventType::MouseButtonPressed : EventType::MouseButtonReleased)
        , button(button) {}

    MouseButton button;
};

} // namespace Nest
