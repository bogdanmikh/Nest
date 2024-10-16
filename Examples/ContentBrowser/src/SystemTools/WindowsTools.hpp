#pragma once

#include "SystemTools.hpp"

class WindowsTools : public SystemTools {
public:
    void open(const std::string &path) override;
    void copyFolder(const std::string &sourcePath, const std::string &newPath) override;
};