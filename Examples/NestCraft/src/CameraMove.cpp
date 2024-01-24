//
// Created by bogdan on 22.01.2024.
//

#include "CameraMove.hpp"

CameraMove::CameraMove(Window* window, Shader *shader, float posX, float posY, float posZ) {
    window = window;
    camera.setShader(shader);
    camera.setPosition(posX, posY, posZ);
    lastPos = window->getCursorPos();
}

void CameraMove::update(double deltaTime) {
    if (window->isKeyPressed(Key::LEFT_SHIFT)) {
        cameraSpeed = 20.f;
    } else cameraSpeed = 5.f;

    if (window->isKeyPressed(Key::W)) {
        camera.translateLocal(0., 0., cameraSpeed * deltaTime);
    }
    if (window->isKeyPressed(Key::S)) {
        camera.translateLocal(0., 0., -cameraSpeed * deltaTime);
    }
    if (window->isKeyPressed(Key::A)) {
        camera.translateLocal(-cameraSpeed * deltaTime, 0., 0.);
    }
    if (window->isKeyPressed(Key::D)) {
        camera.translateLocal(cameraSpeed * deltaTime, 0., 0.);
    }
    if (window->isKeyPressed(Key::SPACE)) {
        camera.translateLocal( 0., cameraSpeed * deltaTime, 0.);
    }
    if (window->isKeyPressed(Key::LEFT_CONTROL)) {
        camera.translateLocal( 0., -cameraSpeed * deltaTime, 0.);
    }

    glm::vec2 cursorPos = window->getCursorPos();
    glm::vec2 diff = lastPos - cursorPos;
    lastPos = cursorPos;
    float mouseSpeed = 0.1f;
    camera.rotate(-diff.y * mouseSpeed, -diff.x * mouseSpeed, 0.f);

    glm::vec2 resolution = window->getSize();
    camera.updateAspectRatio(resolution.x / resolution.y);
}
