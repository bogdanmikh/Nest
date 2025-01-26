#pragma once

#include "Nest/Base/Base.hpp"
#include "Nest/Renderer/Texture.hpp"
#include "Nest/SystemTools/SystemTools.hpp"

#include <filesystem>
#include <map>

using namespace Nest;

class ContentBrowser {
public:
    ContentBrowser();
    void onImGuiRender();
    void setBaseDirectory(const Path &path);

private:
    Path m_baseDirectory;
    Path m_currentDirectory;
    Texture m_directoryIcon;
    Texture m_defaultFileIcon;
    std::unordered_map<std::string, Texture> m_fileIcons;
};