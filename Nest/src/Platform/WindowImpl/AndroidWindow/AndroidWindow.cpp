#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <jni.h>

#include "AndroidWindow.hpp"

#define nativeWindow (ANativeWindow*)handle

namespace Nest {


AndroidWindow::AndroidWindow(ANativeActivity *activity)
        : handle(nullptr), eglDisplay(EGL_NO_DISPLAY),
        eglSurface(EGL_NO_SURFACE), eglContext(EGL_NO_CONTEXT),
        closeRequested(false) {}

AndroidWindow::~AndroidWindow() {
    if (eglDisplay != EGL_NO_DISPLAY) {
        if (eglContext != EGL_NO_CONTEXT) {
            eglDestroyContext(eglDisplay, eglContext);
        }
        if (eglSurface != EGL_NO_SURFACE) {
            eglDestroySurface(eglDisplay, eglSurface);
        }
        eglTerminate(eglDisplay);
    }
    if (nativeWindow) {
        ANativeWindow_release(nativeWindow);
    }
}

void AndroidWindow::init(
    const char *name, uint32_t resolutionX, uint32_t resolutionY, bool fullScreen
) {
    // Initialize EGL
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(eglDisplay, nullptr, nullptr);

    // Set up EGL configuration
    EGLConfig config;
    EGLint numConfigs;
    EGLint attribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };

    eglChooseConfig(eglDisplay, attribs, &config, 1, &numConfigs);

    // Create an EGL window surface
    eglSurface = eglCreateWindowSurface(eglDisplay, config, nativeWindow, nullptr);

    // Create OpenGL ES 3 context
    EGLint contextAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };
    eglContext = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, contextAttribs);

    // Make the context current
    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
}

void AndroidWindow::init(const char *name, bool fullScreen) {
    init(name, 1, 1, fullScreen);
}

bool AndroidWindow::shouldClose() {
    return closeRequested;
}

Vec2 AndroidWindow::getSize() {

}

double AndroidWindow::getTime() {
    return static_cast<double>(time(nullptr));
}

void AndroidWindow::swapBuffers() {
    eglSwapBuffers(eglDisplay, eglSurface);
}

void AndroidWindow::setShouldClose() {
    closeRequested = true;
}

void *AndroidWindow::getNativeHandle() {
    return handle;
}

} // namespace Nest