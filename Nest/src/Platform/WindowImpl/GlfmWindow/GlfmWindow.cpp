#include "GlfmWindow.hpp"

#include "Nest/Base/Base.hpp"
#include <Foundation/Logger.hpp>
#include <Bird/PlatformData.hpp>

namespace Nest {

void GlfmWindow::init(
    const char *name, uint32_t resolutionX, uint32_t resolutionY, bool fullScreen
) {
    glfmMain((GLFMDisplay *)handle);
    glfmSetDisplayConfig(
        (GLFMDisplay *)handle,
        GLFMRenderingAPIOpenGLES2,
        GLFMColorFormatRGBA8888,
        GLFMDepthFormatNone,
        GLFMStencilFormatNone,
        GLFMMultisampleNone
    );
    //    glfmSetRenderFunc((GLFMDisplay*) handle, onDraw);
    //    glfmSetSurfaceDestroyedFunc((GLFMDisplay*) handle, onSurfaceDestroyed);
}

void GlfmWindow::init(const char *name, bool fullScreen) {
    init(name, 1, 1, fullScreen);
}

GlfmWindow::~GlfmWindow() {}

bool GlfmWindow::shouldClose() {
    return true;
}

Vec2 GlfmWindow::getSize() {
    int x, y;
    float xscale = 1, yscale = 1;
    //    glfmGetDisplayScale((GLFMDisplay *)handle);
    glfmGetDisplaySize((GLFMDisplay *)handle, &x, &y);

    return {x * xscale, y * yscale};
}

double GlfmWindow::getTime() {
    return glfmGetTime();
}

void GlfmWindow::swapBuffers() {
    glfmSwapBuffers((GLFMDisplay *)handle);
}

void GlfmWindow::setShouldClose() {}

void *GlfmWindow::getNativeHandle() {
    return handle;
}

} // namespace Nest