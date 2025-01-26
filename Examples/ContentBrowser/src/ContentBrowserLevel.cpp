#include "ContentBrowserLevel.hpp"
#include <filesystem>
#include <imgui.h>
#include <iostream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

ContentBrowserLevel::ContentBrowserLevel() {}

void ContentBrowserLevel::onAttach() {
    panel.setBaseDirectory(fs::current_path().parent_path().parent_path().parent_path());
}

std::string getFolderName(const std::string &path) {
    std::ostringstream res;
    for (int i = path.size(); i >= 0 && path[i] != fs::path::preferred_separator; ++i) {
        res << path[i];
    }
    auto resStr = res.str();
    std::reverse(resStr.begin(), resStr.end());
    return resStr;
}

void ContentBrowserLevel::onUpdate(double deltaTime) {
    panel.onImGuiRender();
}

ContentBrowserLevel::~ContentBrowserLevel() {}

void ContentBrowserLevel::onDetach() {}
