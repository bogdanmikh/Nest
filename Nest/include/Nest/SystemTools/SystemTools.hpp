#pragma once

#include "Nest/Base/Base.hpp"

namespace Nest {

class SystemTools {
public:
    static std::optional<Path> openFileDialog(const char *filter = "All\0*.*\0");
    static std::optional<Path> openFolderDialog(const char *initialFolder = "");
    static std::optional<Path> saveFileDialog(
        const char *filter = "All\0*.*\0",
        const char *defaultPath = nullptr,
        const char *defaultName = nullptr
    );
    static void copyFolder(const Path &sourcePath, const Path &newPath);
    static void open(const Path &path);
    static void show(const Path &path);
};

} // namespace Nest