
#include "ContentBrowser.hpp"
#include "Nest/ImGui/FontAwesome.h"

#include <imgui.h>

ContentBrowser::ContentBrowser()
    : m_baseDirectory()
    , m_currentDirectory()
    , m_defaultFileIcon("Icons/_plain.png")
    , m_directoryIcon("Icons/DirectoryIcon.png")
    , m_fileIcons(20) {
    m_fileIcons.emplace(".avi", Texture("Icons/avi.png"));
    m_fileIcons.emplace(".bmp", Texture("Icons/bmp.png"));
    m_fileIcons.emplace(".c", Texture("Icons/c.png"));
    m_fileIcons.emplace(".cpp", Texture("Icons/cpp.png"));
    m_fileIcons.emplace(".gif", Texture("Icons/gif.png"));
    m_fileIcons.emplace(".h", Texture("Icons/h.png"));
    m_fileIcons.emplace(".hpp", Texture("Icons/hpp.png"));
    m_fileIcons.emplace(".html", Texture("Icons/html.png"));
    m_fileIcons.emplace(".jpg", Texture("Icons/jpg.png"));
    m_fileIcons.emplace(".mp3", Texture("Icons/mp3.png"));
    m_fileIcons.emplace(".mp4", Texture("Icons/mp4.png"));
    m_fileIcons.emplace(".png", Texture("Icons/png.png"));
    m_fileIcons.emplace(".tga", Texture("Icons/tga.png"));
    m_fileIcons.emplace(".tiff", Texture("Icons/tiff.png"));
    m_fileIcons.emplace(".txt", Texture("Icons/txt.png"));
    m_fileIcons.emplace(".wav", Texture("Icons/wav.png"));
    m_fileIcons.emplace(".xml", Texture("Icons/xml.png"));
    m_fileIcons.emplace(".yml", Texture("Icons/yml.png"));
    m_fileIcons.emplace(".zip", Texture("Icons/zip.png"));
}

bool isMouseInsideWindow(ImVec2 windowPos, ImVec2 windowSize) {
    ImVec2 maxSize = windowPos;
    maxSize.x += windowSize.x;
    maxSize.y += windowSize.y;
    return ImGui::IsMouseHoveringRect(windowPos, maxSize);
}

void ContentBrowser::onImGuiRender() {
    static bool showHiddenFiles = true;

    if (m_currentDirectory.empty()) {
        return;
    }

    ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_NoTitleBar);
    if (m_currentDirectory != Nest::Path(m_baseDirectory)) {
        if (ImGui::Button(getString(ICON_ARROW_LEFT).c_str())) {
            m_currentDirectory = m_currentDirectory.parent_path();
        }
    }
    ImGui::SameLine();
    ImGui::Checkbox("Show Hidden Files", &showHiddenFiles);

    static float padding = 8.0f;
    static float thumbnailSize = 90.0f;
    float cellSize = thumbnailSize + padding;

    float ContentBrowserWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(ContentBrowserWidth / cellSize);
    columnCount = std::max(columnCount, 1);
    //    if (!Nest::GlfwEvents::getDropPaths().empty()) {
    //        if (isMouseInsideWindow(ImGui::GetWindowPos(),
    //        ImGui::GetWindowSize())) {
    //            const auto &dropPaths = Nest::GlfwEvents::getDropPaths();
    //            for (const auto &dropPath : dropPaths) {
    //                if (std::filesystem::is_directory(dropPath)) {
    //                    SystemTools::copyFolder(dropPath,
    //                    m_currentDirectory.string()); LOG_INFO("COPY DIR THIS:
    //                    {}, THERE: {}", dropPath, m_currentDirectory.string());
    //                } else {
    //                    std::filesystem::copy(dropPath, m_currentDirectory);
    //                    LOG_INFO(
    //                        "COPY FILE THIS: {}, THERE: {}", dropPath,
    //                        m_currentDirectory.string()
    //                    );
    //                }
    //            }
    //        }
    //    }

    //    ImGui::ShowDemoWindow();

    ImGui::Columns(columnCount, 0, false);
    for (auto &directoryEntry : std::filesystem::directory_iterator(m_currentDirectory)) {
        const auto &path = directoryEntry.path();
        std::string filenameString = path.filename().string();

        if (!showHiddenFiles && filenameString[0] == '.') {
            continue;
        }

        ImGui::PushID(filenameString.c_str());

        Texture *icon;
        if (directoryEntry.is_directory()) {
            icon = &m_directoryIcon;
        } else {
            if (m_fileIcons.find(path.extension().string()) != m_fileIcons.end()) {
                icon = &m_fileIcons[path.extension().string()];
            } else {
                icon = &m_defaultFileIcon;
            }
        }
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        int native = Bird::getNativeTextureHandle(icon->getHandle());
        ImGui::ImageButton((ImTextureID)(intptr_t)native, {thumbnailSize, thumbnailSize});
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            if (directoryEntry.is_directory()) {
                m_currentDirectory /= path.filename();
            }
        }
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Show in Finder")) {
                SystemTools::show(path.c_str());
                // LOG_INFO("PATH: {}", path.c_str());
            }
            ImGui::EndPopup();
        }

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) &&
            !is_directory(path)) {
            SystemTools::open(path);
        }
        ImGui::TextWrapped("%s", filenameString.c_str());
        ImGui::NextColumn();
        ImGui::PopID();
    }
    //    if (ImGui::Button("LOH") && ImGui::BeginPopupContextItem()) {
    //        if (ImGui::Button("Open in Finder")) {
    //            //            SystemTools::openFolderDialog(path.c_str());
    //        }
    //        ImGui::EndPopup();
    //    }
    ImGui::Columns(1);
    ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
    ImGui::SliderFloat("Padding", &padding, 0, 32);
    ImGui::End();
}

void ContentBrowser::setBaseDirectory(const Path &path) {
    m_baseDirectory = path;
    m_currentDirectory = path;
}