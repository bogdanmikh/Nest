#include "CameraMove.hpp"

#include <Nest.hpp>

void CameraMove::onAttach() {
    m_window = Nest::Application::get()->getWindow();
    m_worldCamera = Nest::Application::get()->getWorldCamera();
    m_worldCamera->setPosition(0.0, 0.0, 10.0);
    m_worldCamera->setFieldOfView(glm::radians(60.f));
    cursorLock = false;
}

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
    bool cursorLocked = Application::get()->getWindow()->isCursorLocked();
    if (cursorLocked != cursorLock) {
        cursorLock = cursorLocked;
        resetMouse = true;
    }

    if (!cursorLocked)
        return;

    glm::vec2 cursorPos = {0, 0};
    //    glm::vec2 cursorPos = {Input::getMousePositionX(), Input::getMousePositionY()};
    glm::vec2 diff = lastPos - cursorPos;
    if (resetMouse)
        diff = glm::vec2(0);
    lastPos = cursorPos;
    float mouseSpeed = 0.1f;
    m_worldCamera->rotate(-diff.y * mouseSpeed, -diff.x * mouseSpeed, 0.f);
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
    float coeff = 4.;
    ImGui::SetNextWindowPos({100, 700});
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground |
                                   ImGuiWindowFlags_AlwaysAutoResize;
    ImGui::Begin("Move", nullptr, windowFlags);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.9f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.9f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.9f, 0.0f, 1.0f));
    if (ImGui::Button("Left", {200, 200})) {
        m_worldCamera->translateLocal(-cameraSpeed * dt * coeff, 0., 0);
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    if (ImGui::Button("Right", {200, 200})) {
        m_worldCamera->translateLocal(cameraSpeed * dt * coeff, 0., 0.);
    }
    ImGui::PopStyleColor(3);

    ImGui::End();
#endif
}