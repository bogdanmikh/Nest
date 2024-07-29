
#include "Panel.hpp"

#include <imgui.h>
#include <fstream>
#ifdef PLATFORM_LINUX
#    include "SystemTools/LinuxTools.hpp"
#elif defined(PLATFORM_MACOS)
#    include "SystemTools/MacOSTools.hpp"
#elif PLATFORM_WINDOWS
#    include "SystemTools/WindowsTools.hpp"
#endif

Panel::~Panel() {
    delete m_defaultFileIcon;
    delete m_directoryIcon;
    for (const auto &icon : m_fileIcons) {
        delete icon.second;
    }
    delete m_systemTools;
}

static std::string getString(const char8_t *ptr) {
    return (const char *)ptr;
}

void Panel::start() {
    m_baseDirectory = std::filesystem::current_path();
//    m_currentDirectory = std::filesystem::current_path();
    m_currentDirectory = "/Users/bogdan";
    m_defaultFileIcon = new Texture("Icons/_blank.png", true);
    m_directoryIcon = new Texture("Icons/DirectoryIcon.png", true);
    m_fileIcons.emplace(".cpp", new Texture("Icons/hpp.png", true));
    m_fileIcons.emplace(".hpp", new Texture("Icons/cpp.png", true));
    m_fileIcons.emplace(".h", new Texture("Icons/h.png", true));
    m_fileIcons.emplace(".c", new Texture("Icons/c.png", true));
    m_fileIcons.emplace(".mp3", new Texture("Icons/mp3.png", true));
    m_fileIcons.emplace(".mp4", new Texture("Icons/mp4.png", true));
    m_fileIcons.emplace(".wav", new Texture("Icons/wav.png", true));
    m_fileIcons.emplace(".png", new Texture("Icons/png.png", true));
    m_fileIcons.emplace(".sql", new Texture("Icons/sql.png", true));
    m_fileIcons.emplace(".txt", new Texture("Icons/txt.png", true));
    m_fileIcons.emplace(".yml", new Texture("Icons/yml.png", true));
    m_fileIcons.emplace(".jpg", new Texture("Icons/jpg.png", true));
    m_fileIcons.emplace(".html", new Texture("Icons/html.png", true));
#ifdef PLATFORM_LINUX
    m_systemTools = new LinuxTools;
#elif defined(PLATFORM_MACOS)
    m_systemTools = new MacOSTools;
#elif PLATFORM_WINDOWS
    m_systemTools = new WindowsTools;
#endif
}

bool isMouseInsideWindow(ImVec2 windowPos, ImVec2 windowSize) {
    ImVec2 maxSize = windowPos;
    maxSize.x += windowSize.x;
    maxSize.y += windowSize.y;
    return ImGui::IsMouseHoveringRect(windowPos, maxSize);
}

void Panel::onImGuiRender() {
    static bool showHiddenFiles = true;
    ImGui::Begin("Content Browser");
    if (m_currentDirectory != std::filesystem::path(m_baseDirectory)) {
        if (ImGui::Button(getString(ICON_ARROW_LEFT).c_str())) {
            m_currentDirectory = m_currentDirectory.parent_path();
        }
    }
    ImGui::SameLine();
    ImGui::Checkbox("Show Hidden Files", &showHiddenFiles);

    static float padding = 16.0f;
    static float thumbnailSize = 128.0f;
    float cellSize = thumbnailSize + padding;

    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / cellSize);
    columnCount = std::max(columnCount, 1);
    if (!Events::getDropPaths().empty()) {
        if (isMouseInsideWindow(ImGui::GetWindowPos(), ImGui::GetWindowSize())) {
            const auto &dropPaths = Events::getDropPaths();
            for (const auto &dropPath : dropPaths) {
                if (std::filesystem::is_directory(dropPath)) {
                    m_systemTools->copyFolder(dropPath, m_currentDirectory.string());
                    LOG_INFO("COPY DIR THIS: {}, THERE: {}", dropPath, m_currentDirectory.string());
                } else {
                    std::filesystem::copy(dropPath, m_currentDirectory);
                    LOG_INFO("COPY FILE THIS: {}, THERE: {}", dropPath, m_currentDirectory.string());
                }
            }
        }
    }

    ImGui::Columns(columnCount, 0, false);
    for (auto &directoryEntry : std::filesystem::directory_iterator(m_currentDirectory)) {
        const auto &path = directoryEntry.path();
        std::string filenameString = path.filename().string();
        if (!showHiddenFiles && filenameString[0] == '.')
            continue;

        ImGui::PushID(filenameString.c_str());
        Texture *icon;
        if (directoryEntry.is_directory()) {
            icon = m_directoryIcon;
        } else {
            if (m_fileIcons.find(path.extension().string()) != m_fileIcons.end()) {
                icon = m_fileIcons[path.extension().string()];
            } else {
                icon = m_defaultFileIcon;
            }
        }
        icon->bind();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::ImageButton(
            (ImTextureID)icon->getRendererId(), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0}
        );
        icon->unbind();
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            if (directoryEntry.is_directory()) {
                m_currentDirectory /= path.filename();
            }
        }
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) &&
            !is_directory(path)) {
            m_systemTools->open(path);
        }
        ImGui::TextWrapped("%s", filenameString.c_str());
        ImGui::NextColumn();
        ImGui::PopID();
    }
    ImGui::Columns(1);
    ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
    ImGui::SliderFloat("Padding", &padding, 0, 32);
    ImGui::End();
}
