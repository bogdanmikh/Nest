//
// Created by Bogdan on 23.08.2024.
//

#include "StartMenu.hpp"
#include "Nest/ImGui/ImGuiFonts.hpp"
#include <Nest/Application/Application.hpp>
#include <imgui.h>

void StartMenu::showStartMenu() {
    m_isServer = false;
    m_isClient = false;
    ImGuiIO &io = ImGui::GetIO();
    auto windowSize = io.DisplaySize;
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({windowSize.x, windowSize.y});
    ImGui::Begin(
        "##startmenu",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground
    );
    //    ImGui::Begin("##startmenu", nullptr, ImGuiWindowFlags_NoResize |
    //    ImGuiWindowFlags_NoMove);

    ImVec2 buttonSize(100, 30);

    float spacing = 10.0f;
    float xPos = (windowSize.x - buttonSize.x) / 2.0f; // X coords
    float yPos = (windowSize.y - buttonSize.y) / 2.0f; // Y coords
    /// text
    {
        Nest::Fonts::pushFont("ExtraBig");
        std::string message = "Hello Atoms Game!";

        ImVec2 textSize = ImGui::CalcTextSize(message.c_str());
        ImGui::SetCursorPos({(windowSize.x - textSize.x) / 2, yPos - textSize.y - spacing});
        ImGui::Text("%s", message.c_str());
        Nest::Fonts::popFont();
    }
    /// buttons
    {
        ImGui::SetCursorPos({xPos, yPos});
        if (ImGui::Button("Create Server", buttonSize)) {
            m_isServer = true;
        }

        ImGui::SetCursorPos({xPos, yPos + spacing + buttonSize.y});
        if (ImGui::Button("Connected", buttonSize)) {
            m_isClient = true;
        }
    }
    ImGui::End();
}