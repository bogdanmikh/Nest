//
// Created by bogdan on 30.01.2024.
//

#include <NestUI.hpp>
#include <imgui.h>

#include "Menu.hpp"

Menu::Menu() {
//    ImGui::StyleColorsDark((0.4, 0.4, 0.4, 0.4));
    m_color[0] = 1.0f;
    m_color[1] = 1.0f;
    m_color[2] = 1.0f;
}
bool timeOn = true;
void Menu::update(double deltaTime) {
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = {0.639, 0.639, 0.639, 0.7};

    ImVec2 displaySize = ImGui::GetIO().DisplaySize;
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x / 4, displaySize.y / 4));

    ImGui::Begin("Menu", nullptr);
    ImGui::TextColored({0, 255, 0, 255}, "FPS: %d", Application::getInstance()->getFps());
    const glm::vec3& position = Application::getInstance()->getCamera()->getPosition();
    ImGui::Text("Position: %.2f, %.2f, %.2f", position.x, position.y, position.z);

    ImGui::ColorEdit3("Color", m_color);
//    ImGui::ColorPicker4("Picker", m_color);q
    Application::getInstance()->getCamera()->getShader()->setVec3("u_color", m_color[0], m_color[1], m_color[2]);
//    ImGui::Columns(2);
    if (ImGui::Button((timeOn) ? "Time Off" : "Time On", ImVec2(100, 100))) {
        timeOn = !timeOn;
    }
    if (timeOn) {
        ImGui::Text("Time: %.3f", ImGui::GetTime());
    }
    ImGui::End();
}