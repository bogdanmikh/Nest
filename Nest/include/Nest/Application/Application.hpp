#pragma once

//#include "Nest.hpp"
#include "Nest/Window/Window.hpp"
#include "Nest/NestObjects/Camera.hpp"
#include "Nest/NestObjects/Level.hpp"

class Application {
public:
    ~Application();
    inline static Application* getInstance() {
        return s_instance;
    }

    inline Window* getWindow() {
        return window;
    }

    inline Camera* getCamera() {
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

    Level* currentLevel;
private:
    Application();

    static Application *s_instance;
    Camera* camera;
    Window* window;
    int fps;
    int maximumFps = 60;
    uint64_t deltaTimeMillis = 0;
    int thisSecondFramesCount = 0;
    uint64_t timeMillis;
    uint64_t oneSecondTimeCount = 0;
};