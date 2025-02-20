#include "CameraMove.hpp"
#include "Nest/ImGui/ImGuiFonts.hpp"

#include <imgui_internal.h>
#include <Nest.hpp>

static bool button(Nest::Input::Touch touch, Nest::Vec4 button) {
    float x = touch.x;
    float y = touch.y;

    return x >= button.x && x <= button.x + button.z && y >= button.y && y <= button.y + button.w;
}

void CameraMove::onAttach() {
    m_window = Nest::Application::get()->getWindow();
    m_worldCamera = Nest::Application::get()->getWorldCamera();
    m_worldCamera->setPosition(0.0, 7.5, 100.0);
    m_worldCamera->setFieldOfView(glm::radians(60.f));
    cursorLock = false;
}

void myCustomDrawing(Nest::Vec2 pos, int id) {
    // Получаем текущий ImDrawList для окна
    ImDrawList *draw_list = ImGui::GetBackgroundDrawList();
    // Рисуем круг
    ImVec2 circle_center = ImVec2(pos.x, pos.y);
    float circle_radius = 100.0f;
    draw_list->AddCircle(circle_center, circle_radius, IM_COL32(0, 255, 0, 255)); // Зеленый круг
    char text[20];
    ImFormatString(text, sizeof(text), "%d", id);
    pos.y -= 200;
    Nest::Fonts::pushFont("Large");
    draw_list->AddText(pos, IM_COL32(255, 0, 0, 255), text);
    Nest::Fonts::popFont();
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

    if (!Nest::Application::get()->getWindow()->isCursorLocked())
        return;
    float mouseSpeed;
    Nest::Vec2 delta = {0, 0};
#ifdef PLATFORM_DESKTOP
    mouseSpeed = 0.1f;
    Nest::Vec2 cursorPos = {
        (float)Nest::Input::getMousePositionX(), (float)Nest::Input::getMousePositionY()
    };
    delta = lastPos - cursorPos;
    if (resetMouse)
        delta = Nest::Vec2(0, 0);
    lastPos = cursorPos;
#elif defined(PLATFORM_ANDROID)
    if (Input::touchCount() == 0) {
        lastPos = {0, 0};
        return;
    }
    mouseSpeed = 0.1;
    // pos_x, pos_y, width, height
    Nest::Vec4 buttonLeft(100, 600, 200, 200);
    Nest::Vec4 buttonRight(buttonLeft.x, buttonLeft.y + buttonLeft.w, 200, 200);

    bool found = false;

    for (int i = 0; i < Input::touchCount(); ++i) {
        Input::Touch currentTouch = Input::getTouch(i);
        Nest::Vec2 pos = {currentTouch.x, currentTouch.y};
        //        if (button(currentTouch, buttonLeft)) {
        //            m_worldCamera->translateLocal(0, 0., cameraSpeed * deltaTime * 0.5);
        //        } else if (button(currentTouch, buttonRight)) {
        //            m_worldCamera->translateLocal(0, 0., -cameraSpeed * deltaTime * 0.5);
        //        }
        if (!found && currentTouch.x >= resolution.x / 2) {
            delta = lastPos - pos;
            if (lastPos == Nest::Vec2(0, 0)) {
                delta = {0, 0};
            }
            if (lastPos != pos) {}
            lastPos = pos;
            found = true;
        }
    }
    if (!found) {
        lastPos = {0, 0};
    }
#endif
    m_worldCamera->rotate(-delta.y * mouseSpeed, -delta.x * mouseSpeed, 0.f);
}

void CameraMove::onImGuiRender() {
#ifdef PLATFORM_ANDROID
    for (int i = 0; i < Nest::Input::touchCount(); ++i) {
        Nest::Input::Touch currentTouch = Nest::Input::getTouch(i);
        Nest::Vec2 pos = {currentTouch.x, currentTouch.y};

        //        if (button(currentTouch, buttonLeft)) {
        //            m_worldCamera->translateLocal(0, 0., cameraSpeed * deltaTime * 0.5);
        //        } else if (button(currentTouch, buttonRight)) {
        //            m_worldCamera->translateLocal(0, 0., -cameraSpeed * deltaTime * 0.5);
        //        }
        myCustomDrawing(pos, currentTouch.id);
    }

    float coeff = 0.5;
    ImGui::SetNextWindowPos({100, 600});
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground |
                                   ImGuiWindowFlags_AlwaysAutoResize;
    ImGui::Begin("Move", nullptr, windowFlags);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.9f, 0.0f, 1.0f));
    if (ImGui::Button("##loh", {200, 200})) {}
    if (ImGui::IsItemActive()) {
        m_worldCamera->translateLocal(0, 0., cameraSpeed * dt * coeff);
    }
    ImGui::PopStyleColor();

    //    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    if (ImGui::Button("##pi", {200, 200})) {}
    if (ImGui::IsItemActive()) {
        m_worldCamera->translateLocal(0, 0., -cameraSpeed * dt * coeff);
    }
    ImGui::PopStyleColor();

    ImGui::End();
#endif
}