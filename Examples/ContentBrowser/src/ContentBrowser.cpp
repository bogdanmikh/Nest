#include "ContentBrowser.hpp"
#include <imgui.h>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

ContentBrowser::ContentBrowser() {
    panel.start();
}

void ContentBrowser::start() {}

std::string getFolderName(const std::string &path) {
    std::ostringstream res;
    for (int i = path.size(); i >= 0 && path[i] != fs::path::preferred_separator; ++i) {
        res << path[i];
    }
    auto resStr = res.str();
    std::reverse(resStr.begin(), resStr.end());
    return resStr;
}

void ContentBrowser::update(double deltaTime) {
    /*
    const static std::string path = "/home/bogdan/Documents/VivoProjects";
    static fs::path folder = fs::directory_entry(path);
    ImGui::Begin("File");
    if (ImGui::Button((const char *)ICON_ARROW_LEFT)) {
        folder = folder.parent_path();
        LOG_INFO("PATH: {}", folder.string());
    }
    if (fs::exists(path)) {
        for (const auto &entry : fs::directory_iterator(folder)) {
            auto filename = entry.path().filename();
            if (filename.empty() || filename.string()[0] == '.') {
                continue;
            }
            std::string text = std::string((const char *)ICON_FILE) + " " + filename.string();
            if (fs::is_regular_file(entry.status())) {
                if (filename.string().find(".cpp") != std::string::npos) {
                    text = std::string((const char *)ICON_FILE_CODE_O) + " " + filename.string();
                } else if (filename.string().find(".txt") != std::string::npos) {
                    text = std::string((const char *)ICON_FILE_TEXT) + " " + filename.string();
                }
            } else if (fs::is_directory(entry.status())) {
                text = std::string((const char *)ICON_FOLDER) + " " + filename.string();
            }
            ImGuiTreeNodeFlags flags = 0;
            //                flags |= ImGuiTreeNodeFlags_Selected;
            flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
            flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen;
            flags |= ImGuiTreeNodeFlags_Leaf;
//            flags |= ImGuiTreeNodeFlags_;
            flags |= ImGuiTreeNodeFlags_Bullet;
            ImGui::TreeNodeEx(text.c_str(), flags);
            if (ImGui::IsItemClicked() && fs::is_directory(entry.status())) {
                folder = entry.path();
//                path = path + fs::path::preferred_separator + filename.string();
            }
            if (ImGui::BeginPopupContextItem()) {
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
                if (ImGui::Button("Rename", ImVec2(75, ImGui::GetTextLineHeight() + 5))) {

                } else if (ImGui::Button("Delete", ImVec2(75, ImGui::GetTextLineHeight() + 5))) {
                }
                ImGui::PopStyleVar();
                ImGui::EndPopup();
            }
        }
    } else {
        std::cout << "Path not found\n";
    }
    ImGui::End();
     */
    panel.onImGuiRender();
}

ContentBrowser::~ContentBrowser() {}

void ContentBrowser::detach() {
    LOG_INFO("END");
}
