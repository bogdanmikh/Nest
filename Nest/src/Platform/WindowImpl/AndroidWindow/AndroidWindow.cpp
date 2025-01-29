#include "AndroidWindow.hpp"
#include <Bird/PlatformData.hpp>

#include <android/native_window.h>

namespace Nest {

AndroidWindow::~AndroidWindow() {}

AndroidWindow::AndroidWindow(const char *title, Size size, bool isFullscreen, bool isMaximized) {}

bool AndroidWindow::isFullScreen() {
    return true;
}

void AndroidWindow::setFullScreen(bool isFullScreen) {}

void AndroidWindow::setTitle(const char *title) {}

bool AndroidWindow::isMaximized() {
    return true;
}

void AndroidWindow::setMaximized(bool _isMaximized) {}

void AndroidWindow::setResizable(bool isResizable) {}

void AndroidWindow::setSize(Nest::Size size) {}

void AndroidWindow::pollEvents() {}

bool AndroidWindow::isCursorLocked() {
    return true;
}

void AndroidWindow::toggleCursorLock() {}

void AndroidWindow::setEventQueue(EventQueue *eventQueue) {
    //    m_eventQueue = eventQueue;
}

Size AndroidWindow::getSize() {
    Vec2 res;
    res.width =
        ANativeWindow_getWidth((ANativeWindow *)Bird::PlatformData::get()->nativeWindowHandle);
    res.height =
        ANativeWindow_getHeight((ANativeWindow *)Bird::PlatformData::get()->nativeWindowHandle);
    return res;
}

Size AndroidWindow::getDpi() {
    return {1, 1};
}

double AndroidWindow::getTime() {
    return time(NULL);
}

void AndroidWindow::setCursor(Cursor cursor) {}

const char *AndroidWindow::getClipboardText() {
    return nullptr;
}

void AndroidWindow::setClipboardText(const char *text) {}

void AndroidWindow::setShouldClose() {}

bool AndroidWindow::shouldClose() {
    return false;
}

} // namespace Nest