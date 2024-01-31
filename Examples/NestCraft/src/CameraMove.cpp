#include "CameraMove.hpp"

#include <Nest.hpp>

CameraMove::CameraMove()
    : window(Application::getInstance()->getWindow())
    , camera(Application::getInstance()->getCamera()) {
    camera->setPosition(0.0, 0.0, 5.0);
}

void CameraMove::update(double deltaTime) {
//    if (window->isJustKeyPressed(Key::TAB)) window->toggleCursorLock();

    if (window->isKeyPressed(Key::LEFT_SHIFT)) {
        cameraSpeed = 20.f;
    } else cameraSpeed = 5.f;

    if (window->isKeyPressed(Key::W)) {
        camera->translateLocal(0., 0., cameraSpeed * deltaTime);
    }
    if (window->isKeyPressed(Key::S)) {
        camera->translateLocal(0., 0., -cameraSpeed * deltaTime);
    }
    if (window->isKeyPressed(Key::A)) {
        camera->translateLocal(-cameraSpeed * deltaTime, 0., 0.);
    }
    if (window->isKeyPressed(Key::D)) {
        camera->translateLocal(cameraSpeed * deltaTime, 0., 0.);
    }
    if (window->isKeyPressed(Key::SPACE)) {
        camera->translateLocal( 0., cameraSpeed * deltaTime, 0.);
    }
    if (window->isKeyPressed(Key::LEFT_CONTROL)) {
        camera->translateLocal( 0., -cameraSpeed * deltaTime, 0.);
    }

    glm::vec2 cursorPos = window->getCursorPos();
    glm::vec2 diff = lastPos - cursorPos;
    lastPos = cursorPos;
    float mouseSpeed = 0.1f;
    camera->rotate(-diff.y * mouseSpeed, -diff.x * mouseSpeed, 0.f);



    glm::vec2 resolution = window->getSize();
    camera->updateAspectRatio(resolution.x / resolution.y);
    Renderer::setRenderBufferSize(resolution.x, resolution.y);
}

void CameraMove::draw() {

}