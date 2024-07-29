#include "Profiler.hpp"
#include <imgui.h>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

void Profiler::start() {

}

void Profiler::detach() {}

void Profiler::update(double deltaTime) {
    ImGui::Begin("Loshara");
    ImGui::End();
}