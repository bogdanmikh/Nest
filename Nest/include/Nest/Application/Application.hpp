#pragma once

#include "Nest/Window/Window.hpp"
#include "Nest/NestObjects/Layer.hpp"
#include "Nest/NestObjects/Camera.hpp"

namespace Nest {

class Application final {
public:
    ~Application();
    inline static Application *getInstance() {
        return s_instance;
    }

    inline Window *getWindow() const {
        return window;
    }

    inline Camera *getCamera() const {
        return camera;
    }

    inline int getMaxFps() const {
        return maximumFps;
    }

    inline int getFps() const {
        return fps;
    }

    void loop();
    void close();

    inline void setLayer(Layer *layer) {
        if (currentLayer) {
            currentLayer->detach();
        }
        currentLayer = layer;
        currentLayer->start();
    }

private:
    Application();
    void drawProperties() const;
    void updateViewport();
    static Application *s_instance;

    Window *window;
    Layer *currentLayer;
    Camera *camera;
    glm::vec2 m_lastViewportSize;

    int fps;
    int maximumFps = 60;
    uint64_t deltaTimeMillis = 0;
    int thisSecondFramesCount = 0;
    uint64_t timeMillis = 0;
    uint64_t oneSecondTimeCount = 0;
};

}