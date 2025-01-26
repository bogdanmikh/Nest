#include "GlfwWindow.hpp"
// #include "Nest/GlfwWindow/GlfwNests.hpp"
//
// Created by Admin on 26.01.2022.
//

#include "GlfwWindow.hpp"

#include <Foundation/PlatformDetection.hpp>
#include <Bird/PlatformData.hpp>

namespace Nest {

// We gather version tests as define in order to easily see which features are version-dependent.
#define GLFW_VERSION_COMBINED                                                                      \
    (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 + GLFW_VERSION_REVISION)
#ifdef GLFW_RESIZE_NESW_CURSOR // Let's be nice to people who pulled GLFW between 2019-04-16 (3.4
                               // define) and 2019-11-29 (cursors defines) // FIXME: Remove when
                               // GLFW 3.4 is released?
#    define GLFW_HAS_NEW_CURSORS                                                                   \
        (GLFW_VERSION_COMBINED >= 3400) // 3.4+ GLFW_RESIZE_ALL_CURSOR, GLFW_RESIZE_NESW_CURSOR,
                                        // GLFW_RESIZE_NWSE_CURSOR, GLFW_NOT_ALLOWED_CURSOR
#else
#    define GLFW_HAS_NEW_CURSORS (0)
#endif

void glfwErrorCallback(int error_code, const char *description) {
    LOG_ERROR("GLFW ERROR, code: {}, description: {}", error_code, description);
}

GlfwWindow::~GlfwWindow() {
    glfwDestroyWindow(m_windowHandle);
    glfwTerminate();
}

GlfwWindow::GlfwWindow(const char *title, Size size, bool isFullscreen, bool isMaximized) {
    m_isFullScreen = isFullscreen;
    m_windowSizeBackup = size;
    m_isCursorLocked = false;
    LOG_INFO("Hello GLFW! {}", glfwGetVersionString());
    glfwSetErrorCallback(glfwErrorCallback);
    if (glfwInit() == false) {
        LOG_CRITICAL("GLFW INITIALIZATION ERROR");
        exit(1);
    }
    glfwDefaultWindowHints();
#ifdef PLATFORM_MACOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_MAXIMIZED, isMaximized ? GLFW_TRUE : GLFW_FALSE);
    if (isFullscreen) {
        GLFWmonitor *monitorHandle = glfwGetPrimaryMonitor();
        const GLFWvidmode *vidmode = glfwGetVideoMode(monitorHandle);
        m_windowSizeBackup = {(float)vidmode->width, (float)vidmode->height};
        m_windowHandle =
            glfwCreateWindow(vidmode->width, vidmode->height, title, monitorHandle, nullptr);
    } else {
        m_windowHandle =
            glfwCreateWindow((int)size.width, (int)size.height, title, nullptr, nullptr);
    }
    if (m_windowHandle == nullptr) {
        LOG_CRITICAL("Failed to create the GLFW window");
        exit(1);
    }
    Bird::PlatformData::get()->nativeWindowHandle = m_windowHandle;
    glfwShowWindow(m_windowHandle);
    glfwFocusWindow(m_windowHandle);
    cursors[Cursor::ARROW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    cursors[Cursor::IBEAM] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    cursors[Cursor::CROSSHAIR] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    cursors[Cursor::POINTING_HAND] = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);
    cursors[Cursor::RESIZE_EW] = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
    cursors[Cursor::RESIZE_NS] = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
#if GLFW_HAS_NEW_CURSORS
    cursors[Cursor::RESIZE_NESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
    cursors[Cursor::RESIZE_NWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
    cursors[Cursor::RESIZE_ALL] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
    cursors[Cursor::NOT_ALLOWED] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
#else
    cursors[Cursor::RESIZE_NESW] = cursors[Cursor::ARROW];
    cursors[Cursor::RESIZE_NWSE] = cursors[Cursor::ARROW];
    cursors[Cursor::RESIZE_ALL] = cursors[Cursor::ARROW];
    cursors[Cursor::NOT_ALLOWED] = cursors[Cursor::ARROW];
#endif
}

bool GlfwWindow::isFullScreen() {
    return m_isFullScreen;
}

void GlfwWindow::setFullScreen(bool isFullScreen) {
    m_isFullScreen = isFullScreen;
    GLFWmonitor *monitorHandle = glfwGetPrimaryMonitor();
    const GLFWvidmode *vidmode = glfwGetVideoMode(monitorHandle);
    if (isFullScreen) {
        glfwSetWindowMonitor(
            m_windowHandle, monitorHandle, 0, 0, vidmode->width, vidmode->height, GLFW_DONT_CARE
        );
    } else {
        glfwSetWindowMonitor(
            m_windowHandle,
            nullptr,
            0,
            0,
            m_windowSizeBackup.width,
            m_windowSizeBackup.height,
            GLFW_DONT_CARE
        );
        // Center the window
        glfwSetWindowPos(
            m_windowHandle,
            (vidmode->width - m_windowSizeBackup.width) / 2,
            (vidmode->height - m_windowSizeBackup.height) / 2
        );
    }
}

void GlfwWindow::setTitle(const char *title) {
    glfwSetWindowTitle(m_windowHandle, title);
}

bool GlfwWindow::isMaximized() {
    return glfwGetWindowAttrib(m_windowHandle, GLFW_MAXIMIZED) == GLFW_TRUE;
}

void GlfwWindow::setMaximized(bool _isMaximized) {
    if (isMaximized() == _isMaximized) {
        return;
    }
    if (_isMaximized) {
        glfwMaximizeWindow(m_windowHandle);
    } else {
        glfwRestoreWindow(m_windowHandle);
    }
}

void GlfwWindow::setResizable(bool isResizable) {
    glfwSetWindowAttrib(m_windowHandle, GLFW_RESIZABLE, isResizable);
}

void GlfwWindow::setSize(Nest::Size size) {
    glfwSetWindowSize(m_windowHandle, size.width, size.height);
    m_windowSizeBackup = size;
    int count;
    GLFWmonitor **monitorsHandle = glfwGetMonitors(&count);
    if (count > 1) {
        return;
    }
    const GLFWvidmode *vidmode = glfwGetVideoMode(monitorsHandle[0]);
    glfwSetWindowPos(
        m_windowHandle,
        (vidmode->width - m_windowSizeBackup.width) / 2,
        (vidmode->height - m_windowSizeBackup.height) / 2
    );
}

void GlfwWindow::addEventHandlers() {
    glfwSetWindowUserPointer(m_windowHandle, this);
    glfwSetWindowSizeCallback(m_windowHandle, [](GLFWwindow *windowHandle, int width, int height) {
        GlfwWindow *self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
        self->m_windowSizeBackup = Size(width, height);
        self->m_eventQueue->postSizeEvent(width, height);
    });
    glfwSetKeyCallback(
        m_windowHandle,
        [](GLFWwindow *windowHandle, int key, int scancode, int action, int mods) {
            GlfwWindow *self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
            self->m_eventQueue->postKeyEvent(
                static_cast<Key>(key), action == GLFW_PRESS || action == GLFW_REPEAT
            );
        }
    );
    glfwSetCharCallback(m_windowHandle, [](GLFWwindow *windowHandle, unsigned int c) {
        GlfwWindow *self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
        self->m_eventQueue->postCharEvent(c);
    });
    glfwSetCursorPosCallback(m_windowHandle, [](GLFWwindow *windowHandle, double x, double y) {
        GlfwWindow *self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
        self->m_eventQueue->postMouseEvent(x, y);
    });
    glfwSetScrollCallback(
        m_windowHandle,
        [](GLFWwindow *windowHandle, double xoffset, double yoffset) {
            GlfwWindow *self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
            self->m_eventQueue->postScrollEvent(xoffset, yoffset);
        }
    );
    glfwSetMouseButtonCallback(
        m_windowHandle,
        [](GLFWwindow *windowHandle, int button, int action, int mods) {
            GlfwWindow *self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
            self->m_eventQueue->postMouseButtonEvent(
                static_cast<MouseButton>(button), action == GLFW_PRESS
            );
        }
    );
    glfwSetWindowCloseCallback(m_windowHandle, [](GLFWwindow *windowHandle) {
        GlfwWindow *self = static_cast<GlfwWindow *>(glfwGetWindowUserPointer(windowHandle));
        self->m_eventQueue->postWindowCloseEvent();
    });
    int width, height;
    glfwGetWindowSize(m_windowHandle, &width, &height);
    m_windowSizeBackup = Size(width, height);
    m_eventQueue->postSizeEvent(width, height);
}

void GlfwWindow::pollEvents() {
    glfwPollEvents();
}

bool GlfwWindow::isCursorLocked() {
    return m_isCursorLocked;
}

void GlfwWindow::toggleCursorLock() {
    m_isCursorLocked = !m_isCursorLocked;
    resetCursorPos();
    glfwSetInputMode(
        m_windowHandle, GLFW_CURSOR, m_isCursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
    );
}

void GlfwWindow::resetCursorPos() {
    int x = m_windowSizeBackup.width / 2;
    int y = m_windowSizeBackup.height / 2;
    glfwSetCursorPos(m_windowHandle, x, y);
    m_eventQueue->postMouseEvent(x, y);
}

void GlfwWindow::setEventQueue(EventQueue *eventQueue) {
    m_eventQueue = eventQueue;
    addEventHandlers();
}

Size GlfwWindow::getSize() {
    return m_windowSizeBackup;
}

Size GlfwWindow::getDpi() {
    float xscale, yscale;
    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);
    return Size(xscale, yscale);
}

double GlfwWindow::getTime() {
    return glfwGetTime();
}

void GlfwWindow::setCursor(Cursor cursor) {
    GLFWcursor *glfwCursor = cursors[cursor];
    glfwSetCursor(m_windowHandle, glfwCursor);
}

const char *GlfwWindow::getClipboardText() {
    return glfwGetClipboardString((GLFWwindow *)m_windowHandle);
}

void GlfwWindow::setClipboardText(const char *text) {
    glfwSetClipboardString(m_windowHandle, text);
}

void GlfwWindow::setShouldClose() {
    glfwSetWindowShouldClose((GLFWwindow *)m_windowHandle, true);
}

bool GlfwWindow::shouldClose() {
    return glfwWindowShouldClose((GLFWwindow *)m_windowHandle);
}

} // namespace Nest
