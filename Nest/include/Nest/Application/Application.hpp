#pragma once

#include "ApplicationStartupSettings.hpp"
#include "Nest/Application/Layer.hpp"
#include "Nest/GameLogic/Components/WorldCamera.hpp"
#include "Nest/Window/Window.hpp"
#include "Nest/ImGui/ImGuiLayer.hpp"

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

    void processEvents();

    EventQueue *getEventQueue();

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
    inline void close() {
        m_isApplicationShouldClose = true;
    }

    inline void setLayer(Layer *layer) {
        m_layer = layer;
    }

private:
    void windowSizeChanged(Size size);
    static Application *s_instance;

    Window *m_window;
    Layer *m_layer;
    EventQueue m_eventQueue;
    WorldCamera *m_worldCamera;

    ImGuiLayer *m_ImGuiLayer;

    bool m_isApplicationShouldClose;
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