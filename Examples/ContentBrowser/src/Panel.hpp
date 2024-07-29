#pragma once

#include <Nest.hpp>
#include <filesystem>
#include <map>
#include "SystemTools/SystemTools.hpp"

class Panel {
public:
    Panel() = default;
    ~Panel();
    void start();
    void onImGuiRender();
private:
    SystemTools *m_systemTools;
    std::filesystem::path m_baseDirectory;
    std::filesystem::path m_currentDirectory;
    Texture *m_directoryIcon;
    Texture *m_defaultFileIcon;
    std::map<std::string, Texture *> m_fileIcons;
};