#include "Panda/ImGui/ImGuiLayer.hpp"

#include <imgui.h>

#include "Panda/Application/Application.hpp"

#include "Panda/ImGui/FontAwesome.h"
#include "Panda/ImGui/Colors.hpp"
#include "Panda/ImGui/ImGuiFonts.hpp"
// ImGui platform impl
#include "imgui_impl_panda.hpp"
// ImGui renderer impl
#include "imgui_impl_miren.hpp"

namespace Panda {

ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer") {}

void ImGuiLayer::onAttach() {
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

    FontConfiguration fontBold;
    fontBold.fontName = "Bold";
    fontBold.fileName = "SF-Compact/SF-Compact-Display-Bold.otf";
    fontBold.size = 17.0f;
    Fonts::add(fontBold);

    FontConfiguration fontLarge;
    fontLarge.fontName = "Large";
    fontLarge.fileName = "SF-Compact/SF-Compact-Display-Regular.otf";
    fontLarge.size = 22.0f;
    Fonts::add(fontLarge);

    FontConfiguration fontDefault;
    fontDefault.fontName = "Default";
    fontDefault.fileName = "SF-Compact/SF-Compact-Display-Regular.otf";
    fontDefault.size = 17.0f;
    Fonts::add(fontDefault, true);

    static const ImWchar s_fontAwesomeRanges[] = {ICON_MIN, ICON_MAX, 0};
    FontConfiguration fontAwesome;
    fontAwesome.fontName = "FontAwesome";
    fontAwesome.fileName = "FontAwesome/fontawesome-webfont.ttf";
    fontAwesome.size = 17.0f;
    fontAwesome.glyphRanges = s_fontAwesomeRanges;
    fontAwesome.mergeWithLast = true;
    Fonts::add(fontAwesome);

    FontConfiguration fontSmall;
    fontSmall.fontName = "Small";
    fontSmall.fileName = "SF-Compact/SF-Compact-Display-Medium.otf";
    fontSmall.size = 14.0f;
    Fonts::add(fontSmall);

    FontConfiguration fontExtraSmall;
    fontExtraSmall.fontName = "ExtraSmall";
    fontExtraSmall.fileName = "SF-Compact/SF-Compact-Display-Medium.otf";
    fontExtraSmall.size = 10.0f;
    Fonts::add(fontExtraSmall);

    // ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();
    setDarkThemeColors();

    // Setup Platform/Renderer bindings
    ImGui_ImplPanda_Init();
    ImGui_ImplMiren_Init();
}

void ImGuiLayer::onDetach() {
    ImGui_ImplMiren_Shutdown();
    ImGui_ImplPanda_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::onEvent(Event *event) {
    ImGui_ImplPanda_HandleEvent(event);
    if (!m_blockEvents) {
        event->isHandled = false;
    }
}

void ImGuiLayer::begin(double deltaTime) {
    ImGui_ImplPanda_NewFrame(deltaTime);
    ImGui_ImplMiren_NewFrame();
    ImGui::NewFrame();
    // ImGuizmo::BeginFrame();
}

void ImGuiLayer::end() {
    // Rendering
    ImGui::Render();
    ImGui_ImplMiren_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::setDarkThemeColors() {
    auto &colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);

    // Headers
    colors[ImGuiCol_Header] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);

    // Buttons
    colors[ImGuiCol_Button] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.3805f, 0.381f, 1.0f);
    colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.2805f, 0.281f, 1.0f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);

    // Resize Grip
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);

    // Check Mark
    colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);

    // Slider
    colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
}

} // namespace Panda
