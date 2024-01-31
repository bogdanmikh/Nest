#include <filesystem>

#include "ImGui.hpp"

#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

#include "imgui.h"

#include <iostream>

void ImGui_Init(void* glfwWindowHandle) {
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    float fontSize = 18.0f;

    std::filesystem::path currentPath = std::filesystem::current_path().parent_path();
    std::string path = currentPath.string();
	io.FontDefault = io.Fonts->AddFontFromFileTTF((path + "/NestUI/ImGui/res/ProggyTiny.ttf").c_str(), fontSize);

    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(glfwWindowHandle), true);
    ImGui_ImplOpenGL3_Init();
}

void ImGui_NewFrame() {
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
}

void ImGui_EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGui_Shutdown() {
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}
