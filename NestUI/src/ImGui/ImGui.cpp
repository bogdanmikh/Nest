#include "imgui_impl/imgui_impl_glfw.h"
#include <imgui.h>
#include "imgui_impl/imgui_impl_opengl3.h"

#include <imgui.h>

void ImGui_Init(void* glfwWindowHandle) {
    ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	float fontSize = 18.0f;
	io.FontDefault = io.Fonts->AddFontFromFileTTF("../res/fonts/ProggyTiny.ttf", fontSize);

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
