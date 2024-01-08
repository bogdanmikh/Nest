#include "ImGui/ImGuIObj.hpp"

ImGuIObj::ImGuIObj(const char* name, const ImVec2 &displaySize, const ImVec2 &position) {
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(displaySize);
    ImGui::Begin(name, nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);
}
