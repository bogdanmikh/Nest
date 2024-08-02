#pragma once

#include "Nest/Window/Window.hpp"
#include "Nest/Application/Layer.hpp"
#include "ApplicationStartupSettings.hpp"

namespace Nest {

class Application final {
public:
    Application(ApplicationStartupSettings &settings);
    ~Application();

    inline static Application *get() {
        return s_instance;
    }

    inline Window *getWindow() const {
        return m_window;
    }

    inline int getMaxFps() const {
        return maximumFps;
    }

    inline int getFps() const {
        return fps;
    }

    void loop();
    void close();

    inline void setLayer(Layer* layer) {
         m_layer = layer;
    }

private:
    void updateViewport(Size size);
    static Application *s_instance;

    Window *m_window;
    Layer *m_layer;

    Vec2 m_lastViewportSize;

    int fps;
    int maximumFps = 60;
    uint64_t deltaTimeMillis = 0;
    int thisSecondFramesCount = 0;
    uint64_t timeMillis = 0;
    uint64_t oneSecondTimeCount = 0;
};

}