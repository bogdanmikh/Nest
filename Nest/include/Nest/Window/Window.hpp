#pragma once

#include "Nest/Base/Base.hpp"
#include "Nest/Events/EventQueue.hpp"
#include "Nest/Window/Cursor.hpp"

namespace Nest {

class Window {
public:
    virtual ~Window() = default;
    virtual void setTitle(const char *title) = 0;
    virtual bool isFullScreen() = 0;
    virtual void setFullScreen(bool isFullScreen) = 0;
    virtual bool isMaximized() = 0;
    virtual void setMaximized(bool isMaximized) = 0;
    virtual void setResizable(bool isResizable) = 0;
    virtual void setSize(Size size) = 0;
    virtual void pollEvents() = 0;
    virtual void toggleCursorLock() = 0;
    virtual bool isCursorLocked() = 0;
    virtual void setCursor(Cursor cursor) = 0;
    virtual void setEventQueue(EventQueue *eventQueue) = 0;
    virtual Size getSize() = 0;
    virtual Size getDpi() = 0;
    virtual const char *getClipboardText() = 0;
    virtual double getTime() = 0;
    virtual void setClipboardText(const char *text) = 0;

    virtual bool shouldClose() = 0;
    virtual void setShouldClose() = 0;
};

} // namespace Nest