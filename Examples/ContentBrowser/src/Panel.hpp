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
    Nest::Path m_baseDirectory;
    Nest::Path m_currentDirectory;
    Texture *m_directoryIcon;
    Texture *m_defaultFileIcon;
    std::map<std::string, Texture *> m_fileIcons;
};