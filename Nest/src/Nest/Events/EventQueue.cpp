//
// Created by Admin on 19.02.2022.
//

#include "Nest/Events/EventQueue.hpp"
#include "Nest/Events/KeyEvents.hpp"
#include "Nest/Events/MouseEvents.hpp"
#include "Nest/Events/TouchEvents.hpp"
#include "Nest/Events/WindowEvents.hpp"

namespace Nest {

EventQueue::EventQueue()
    : m_events(30000) {}

void EventQueue::finishWriting() {
    m_events.finishWriting();
}

void EventQueue::reset() {
    m_events.reset();
}

Event *EventQueue::poll() {
    return static_cast<Event *>(m_events.read());
}

void EventQueue::postMouseButtonEvent(MouseButton button, bool pressed) {
    MouseKeyEvent event(button, pressed);
    m_events.write(event);
}

void EventQueue::postSizeEvent(uint32_t width, uint32_t height) {
    WindowResizeEvent event(width, height);
    m_events.write(event);
}

void EventQueue::postKeyEvent(Key key, bool down) {
    if (down) {
        KeyPressedEvent event(key);
        m_events.write(event);
    } else {
        KeyReleasedEvent event(key);
        m_events.write(event);
    }
}

void EventQueue::postCharEvent(uint32_t c) {
    CharacterInputEvent event(c);
    m_events.write(event);
}

void EventQueue::postMouseEvent(double x, double y) {
    MouseMovedEvent event(x, y);
    m_events.write(event);
}

void EventQueue::postScrollEvent(double xoffset, double yoffset) {
    MouseScrolledEvent event(xoffset, yoffset);
    m_events.write(event);
}

void EventQueue::postWindowCloseEvent() {
    WindowCloseEvent event;
    m_events.write(event);
}

void EventQueue::postTouchBeganEvent(int id, float x, float y) {
    TouchBeganEvent event(id, x, y);
    m_events.write(event);
}

void EventQueue::postTouchMovedEvent(int id, float x, float y) {
    TouchMovedEvent event(id, x, y);
    m_events.write(event);
}

void EventQueue::postTouchEndedEvent(int id, float x, float y) {
    TouchEndedEvent event(id, x, y);
    m_events.write(event);
}

} // namespace Nest
