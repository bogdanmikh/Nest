#pragma once

#include "ApplicationStartupSettings.hpp"
#include "Nest/Application/Layer.hpp"
#include "Nest/GameLogic/Components/WorldCamera.hpp"
#include "Nest/Window/Events.hpp"
#include "Nest/Window/Window.hpp"

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

    inline Events *getEvents() {
        return m_events;
    }

    inline WorldCamera *getWorldCamera() const {
        return m_worldCamera;
    }

    inline int getMaxFps() const {
        return m_maximumFps;
    }

    inline int getFps() const {
        return fps;
    }

    void loop();
    void close();

    inline void setLayer(Layer *layer) {
        m_layer = layer;
    }

private:
    void updateViewport(Size size);
    static Application *s_instance;

    Window *m_window;
    Layer *m_layer;
    Events *m_events;
    WorldCamera *m_worldCamera;

    Vec2 m_lastViewportSize;

    int fps;
    uint64_t m_timeMillis;
    // Таймер до 1 секундны для подсчета FPS (в миллисекундах)
    uint64_t m_oneSecondTimeCount;
    // Время после отрисовки предыдущего кадра
    uint64_t m_deltaTimeMillis;
    int m_thisSecondFramesCount;
    // Ограничение по FPS
    int m_maximumFps;
};

} // namespace Nest