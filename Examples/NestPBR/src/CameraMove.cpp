#include "CameraMove.hpp"

#include <Nest.hpp>

void CameraMove::onAttach() {
    m_window = Nest::Application::get()->getWindow();
    m_worldCamera = Nest::Application::get()->getWorldCamera();
    m_worldCamera->setPosition(0.0, 0.0, 8.0);
    m_worldCamera->setFieldOfView(glm::radians(60.f));
    cursorLock = false;
}

double dt;

void CameraMove::onUpdate(double deltaTime) {
    using namespace Nest;
    dt = deltaTime;
    if (Input::isKeyPressed(Key::LEFT_SHIFT)) {
        cameraSpeed = 30.f;
    } else {
        cameraSpeed = 20.f;
    }
    if (deltaTime >= 1) {
        LOG_ERROR("Delta: {}", deltaTime);
    }
    if (Input::isKeyPressed(Key::W)) {
        m_worldCamera->translateLocal(0., 0., cameraSpeed * deltaTime);
    }
    if (Input::isKeyPressed(Key::S)) {
        m_worldCamera->translateLocal(0., 0., -cameraSpeed * deltaTime);
    }
    if (Input::isKeyPressed(Key::A)) {
        m_worldCamera->translateLocal(-cameraSpeed * deltaTime, 0., 0.);
    }
    if (Input::isKeyPressed(Key::D)) {
        m_worldCamera->translateLocal(cameraSpeed * deltaTime, 0., 0.);
    }
    if (Input::isKeyPressed(Key::SPACE)) {
        m_worldCamera->translateLocal(0., cameraSpeed * deltaTime, 0.);
    }
    if (Input::isKeyPressed(Key::LEFT_CONTROL)) {
        m_worldCamera->translateLocal(0., -cameraSpeed * deltaTime, 0.);
    }

    glm::vec2 resolution = m_window->getSize();
    m_worldCamera->updateAspectRatio(resolution.x / resolution.y);

    bool resetMouse = false;
    if (Nest::Application::get()->getWindow()->isCursorLocked() != cursorLock) {
        cursorLock = Nest::Application::get()->getWindow()->isCursorLocked();
        resetMouse = true;
    }

    if (!Nest::Application::get()->getWindow()->isCursorLocked())
        return;
    float mouseSpeed;
    glm::vec2 delta = {0, 0};
#ifdef PLATFORM_DESKTOP
    mouseSpeed = 0.1f;
    glm::vec2 cursorPos = {Nest::Input::getMousePositionX(), Nest::Input::getMousePositionY()};
    delta = lastPos - cursorPos;
    if (resetMouse)
        delta = glm::vec2(0);
    lastPos = cursorPos;
#elif defined(PLATFORM_ANDROID)
    mouseSpeed = 0.1;
    if (Input::touchCount() == 0) {
        delta = {0, 0};
        lastPos = {0, 0};
    } else {
        Input::Touch touch(0, 0, 0);
        bool find = false;
        for (int i = 0; i < Input::touchCount(); ++i) {
            Input::Touch currentTouch = Input::getTouch(i);
            Nest::Size resolution = Application::get()->getWindow()->getSize();
            if (currentTouch.x >= resolution.x / 2) {
                touch = currentTouch;
                find = true;
                break;
            }
        }
        glm::vec2 touchPos = {touch.x, touch.y};
        if (!find) {
            delta = {0, 0};
        } else {
            if (lastPos != glm::vec2(0, 0)) {
                delta = lastPos - touchPos;
            }
        }
        lastPos = touchPos;
    }
#endif
    m_worldCamera->rotate(-delta.y * mouseSpeed, -delta.x * mouseSpeed, 0.f);
}

void CameraMove::onImGuiRender() {
    ImGui::Begin("Camera stats");
    ImGui::Text(
        "Pos x: %f, y: %f, z: %f",
        m_worldCamera->getPosition().x,
        m_worldCamera->getPosition().y,
        m_worldCamera->getPosition().z
    );
    ImGui::End();
#ifdef PLATFORM_ANDROID
    float coeff = 0.5;
    ImGui::SetNextWindowPos({100, 600});
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground |
                                   ImGuiWindowFlags_AlwaysAutoResize;
    ImGui::Begin("Move", nullptr, windowFlags);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.9f, 0.0f, 1.0f));
    if (ImGui::Button("##loh", {200, 200})) {
    }
    if (ImGui::IsItemActive()) {
        m_worldCamera->translateLocal(0, 0., cameraSpeed * dt * coeff);
    }
    ImGui::PopStyleColor();

//    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    if (ImGui::Button("##pi", {200, 200})) {
    }
    if (ImGui::IsItemActive()) {
        m_worldCamera->translateLocal(0, 0., -cameraSpeed * dt * coeff);
    }
    ImGui::PopStyleColor();

    ImGui::End();
#endif
}