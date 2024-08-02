#include "CameraMove.hpp"

#include <Nest.hpp>

void CameraMove::onAttach() {
    m_window = Nest::Application::get()->getWindow();
    m_worldCamera = Nest::Application::get()->getWorldCamera();
    m_worldCamera->setPosition(0.0, 0.0, 10.0);
    cursorLock = false;
}

void CameraMove::onUpdate(double deltaTime) {
    using namespace Nest;
    if (Events::isKeyPressed(Key::LEFT_SHIFT)) {
        cameraSpeed = 30.f;
    } else {
        cameraSpeed = 20.f;
    }
    if (deltaTime >= 1) {
        LOG_ERROR("Delta: {}", deltaTime);
    }
    if (Events::isKeyPressed(Key::W)) {
        m_worldCamera->translateLocal(0., 0., cameraSpeed * deltaTime);
    }
    if (Events::isKeyPressed(Key::S)) {
        m_worldCamera->translateLocal(0., 0., -cameraSpeed * deltaTime);
    }
    if (Events::isKeyPressed(Key::A)) {
        m_worldCamera->translateLocal(-cameraSpeed * deltaTime, 0., 0.);
    }
    if (Events::isKeyPressed(Key::D)) {
        m_worldCamera->translateLocal(cameraSpeed * deltaTime, 0., 0.);
    }
    if (Events::isKeyPressed(Key::SPACE)) {
        m_worldCamera->translateLocal(0., cameraSpeed * deltaTime, 0.);
    }
    if (Events::isKeyPressed(Key::LEFT_CONTROL)) {
        m_worldCamera->translateLocal(0., -cameraSpeed * deltaTime, 0.);
    }

    glm::vec2 resolution = m_window->getSize();
    m_worldCamera->updateAspectRatio(resolution.x / resolution.y);
    NestRen::Rect viewportSize;
    viewportSize.size = resolution;
    NestRen::setViewport(0, viewportSize);
    bool resetMouse = false;
    if (Events::isCursorLocked() != cursorLock) {
        cursorLock = Events::isCursorLocked();
        resetMouse = true;
    }

    if (!Events::isCursorLocked())
        return;

    glm::vec2 cursorPos = Events::getCursorPos();
    glm::vec2 diff = lastPos - cursorPos;
    if (resetMouse)
        diff = glm::vec2(0);
    lastPos = cursorPos;
    float mouseSpeed = 0.1f;
    m_worldCamera->rotate(-diff.y * mouseSpeed, -diff.x * mouseSpeed, 0.f);
}

void CameraMove::onImGuiRender() {
    ImGui::Begin("Camera stats");
    ImGui::Text("Pos x: %f, y: %f, z: %f", m_worldCamera->getPosition().x, m_worldCamera->getPosition().y, m_worldCamera->getPosition().z);
    ImGui::End();
}