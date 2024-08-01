#pragma once

#include <imgui.h> // IMGUI_IMPL_API

#include <Panda/Events/Event.hpp>

// Backend API
IMGUI_IMPL_API bool ImGui_ImplPanda_Init();
IMGUI_IMPL_API void ImGui_ImplPanda_Shutdown();
IMGUI_IMPL_API void ImGui_ImplPanda_HandleEvent(Panda::Event *event);
IMGUI_IMPL_API void ImGui_ImplPanda_NewFrame(double deltaTime);