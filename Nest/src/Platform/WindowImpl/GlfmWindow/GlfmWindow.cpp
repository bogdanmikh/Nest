#include "GlfmWindow.hpp"
// #include "Nest/GlfmWindow/GlfwEvents.hpp"

#include "Nest/Base/Base.hpp"
#include <Foundation/Logger.hpp>
#include <Bird/PlatformData.hpp>

namespace Nest {

void GlfmWindow::init(
    const char *name, uint32_t resolutionX, uint32_t resolutionY, bool fullScreen
) {
    glfmSetDisplayConfig((GLFMDisplay* )handle,
                         GLFMRenderingAPIOpenGLES2,
                         GLFMColorFormatRGBA8888,
                         GLFMDepthFormatNone,
                         GLFMStencilFormatNone,
                         GLFMMultisampleNone);
    glfmSetSurfaceDestroyedFunc((GLFMDisplay*) handle, onSurfaceDestroyed);
}

void GlfmWindow::init(const char *name, bool fullScreen) {
    init(name, 1, 1, fullScreen);
}

GlfmWindow::~GlfmWindow() {
    glfmMain((GLFMDisplay* )handle);
}

bool GlfmWindow::shouldClose() {

}

Vec2 GlfmWindow::getSize() {
    int x, y;
    float xscale, yscale;

    return {x * xscale, y * yscale};
}

double GlfmWindow::getTime() {
    return glfwGetTime();
}

void GlfmWindow::swapBuffers() {
    glfmSwapBuffers((GLFMDisplay* )handle);
}

void GlfmWindow::setShouldClose() {

}

void *GlfmWindow::getNativeHandle() {
    return handle;
}

} // namespace Nest