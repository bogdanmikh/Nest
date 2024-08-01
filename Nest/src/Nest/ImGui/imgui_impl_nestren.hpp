#pragma once
#include <imgui.h>

namespace Nest {
// Backend API
IMGUI_IMPL_API bool ImGui_ImplNestRen_Init();
IMGUI_IMPL_API void ImGui_ImplNestRen_Shutdown();
IMGUI_IMPL_API void ImGui_ImplNestRen_NewFrame();
IMGUI_IMPL_API void ImGui_ImplNestRen_RenderDrawData(ImDrawData *draw_data);

// (Optional) Called by Init/NewFrame/Shutdown
IMGUI_IMPL_API bool ImGui_ImplNestRen_CreateFontsTexture();
IMGUI_IMPL_API void ImGui_ImplNestRen_DestroyFontsTexture();
IMGUI_IMPL_API bool ImGui_ImplNestRen_CreateDeviceObjects();
IMGUI_IMPL_API void ImGui_ImplNestRen_DestroyDeviceObjects();

}
