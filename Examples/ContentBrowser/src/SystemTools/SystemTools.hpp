#pragma once

#include <string>

struct SystemTools {
    virtual ~SystemTools() = default;
    virtual void open(const std::string &path) = 0;
    virtual void copyFolder(const std::string &sourcePath, const std::string &newPath) = 0;
};