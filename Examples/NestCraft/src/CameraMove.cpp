#include "CameraMove.hpp"

#include <Nest.hpp>

CameraMove::CameraMove()
    : window(Application::getInstance()->getWindow())
    , camera(Application::getInstance()->getCamera()) {
    camera->setPosition(0.0, 10.0, 5.0);
//    camera->rotate(20, 90, 0.f);
    cursorLock = false;
}

void CameraMove::update(double deltaTime) {
    if (Events::isKeyPressed(Key::LEFT_SHIFT)) {
        cameraSpeed = 20.f;
    } else {
        cameraSpeed = 5.f;
    }

    if (Events::isKeyPressed(Key::W)) {
        camera->translateLocal(0., 0., cameraSpeed * deltaTime);
    }
    if (Events::isKeyPressed(Key::S)) {
        camera->translateLocal(0., 0., -cameraSpeed * deltaTime);
    }
    if (Events::isKeyPressed(Key::A)) {
        camera->translateLocal(-cameraSpeed * deltaTime, 0., 0.);
    }
    if (Events::isKeyPressed(Key::D)) {
        camera->translateLocal(cameraSpeed * deltaTime, 0., 0.);
    }
    if (Events::isKeyPressed(Key::SPACE)) {
        camera->translateLocal( 0., cameraSpeed * deltaTime, 0.);
    }
    if (Events::isKeyPressed(Key::LEFT_CONTROL)) {
        camera->translateLocal(0., -cameraSpeed * deltaTime, 0.);
    }

    glm::vec2 resolution = window->getSize();
    camera->updateAspectRatio(resolution.x / resolution.y);
    Renderer::setRenderBufferSize(resolution.x, resolution.y);
    bool resetMouse = false;
    if (Events::isCursorLocked() != cursorLock) {
        cursorLock = Events::isCursorLocked();
        resetMouse = true;
    }
    if (!Events::isCursorLocked()) return;

    glm::vec2 cursorPos = Events::getCursorPos();
    glm::vec2 diff = lastPos - cursorPos;
    if (resetMouse) diff = glm::vec2(0);
    lastPos = cursorPos;
    float mouseSpeed = 0.1f;
    camera->rotate(-diff.y * mouseSpeed, -diff.x * mouseSpeed, 0.f);
}

void CameraMove::draw() {}