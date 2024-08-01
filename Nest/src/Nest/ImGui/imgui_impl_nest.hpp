#pragma once

#include <imgui.h> // IMGUI_IMPL_API
#include "Nest/Window/Events.hpp"

namespace Nest {

// Backend API
IMGUI_IMPL_API bool ImGui_ImplNest_Init();
IMGUI_IMPL_API void ImGui_ImplNest_Shutdown();
//IMGUI_IMPL_API void ImGui_ImplNest_HandleEvent(::Event *event);
IMGUI_IMPL_API void ImGui_ImplNest_NewFrame(double deltaTime);

}