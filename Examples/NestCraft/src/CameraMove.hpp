#pragma once

#include <Nest.hpp>

class CameraMove : public GameObject {
public:
    CameraMove();
    void update(double deltaTime) override;
    void draw() override;
private:
    glm::vec2 lastPos;
    float cameraSpeed = 5.f;

    Window* window;
    Camera* camera;
};
