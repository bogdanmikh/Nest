#pragma once

#include <Nest.hpp>

class CameraMove {
public:
    void init();
    void update(double deltaTime);

private:
    bool cursorLock;
    glm::vec2 lastPos;
    float cameraSpeed = 5.f;

    Window *window;
    Camera *camera;
};
