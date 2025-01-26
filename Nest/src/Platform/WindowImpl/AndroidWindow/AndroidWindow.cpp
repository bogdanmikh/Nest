#include "AndroidWindow.hpp"
#include <Bird/PlatformData.hpp>

#include <android/native_window.h>
namespace Nest {

AndroidWindow::~AndroidWindow() {}

void AndroidWindow::init(
    const char *name, uint32_t resolutionX, uint32_t resolutionY, bool fullScreen
) {
    handle = Bird::PlatformData::get()->nativeWindowHandle;
}

void AndroidWindow::init(const char *name, bool fullScreen) {
    init(name, 1, 1, fullScreen);
}

bool AndroidWindow::shouldClose() {
    return false;
}

Vec2 AndroidWindow::getSize() {
    Vec2 res;
    res.width =
        ANativeWindow_getWidth((ANativeWindow *)Bird::PlatformData::get()->nativeWindowHandle);
    res.height =
        ANativeWindow_getHeight((ANativeWindow *)Bird::PlatformData::get()->nativeWindowHandle);
    return res;
}

double AndroidWindow::getTime() {
    return static_cast<double>(time(nullptr));
}

void AndroidWindow::swapBuffers() {}

void AndroidWindow::setShouldClose() {}

void *AndroidWindow::getNativeHandle() {
    return handle;
}

} // namespace Nest