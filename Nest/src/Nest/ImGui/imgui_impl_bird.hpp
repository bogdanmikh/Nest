#pragma once
#include <imgui.h> // IMGUI_IMPL_API

// Backend API
IMGUI_IMPL_API bool ImGui_ImplBird_Init();
IMGUI_IMPL_API void ImGui_ImplBird_Shutdown();
IMGUI_IMPL_API void ImGui_ImplBird_NewFrame();
IMGUI_IMPL_API void ImGui_ImplBird_RenderDrawData(ImDrawData *draw_data);

// (Optional) Called by Init/NewFrame/Shutdown
IMGUI_IMPL_API bool ImGui_ImplBird_CreateFontsTexture();
IMGUI_IMPL_API void ImGui_ImplBird_DestroyFontsTexture();
IMGUI_IMPL_API bool ImGui_ImplBird_CreateDeviceObjects();
IMGUI_IMPL_API void ImGui_ImplBird_DestroyDeviceObjects();
