#pragma once

#include <imgui.h>
#include <string>

namespace Nest {

struct FontConfiguration {
    std::string fontName;
    std::string fileName;
    float size = 16.0f;
    const ImWchar *glyphRanges = nullptr;
    bool mergeWithLast = false;
};

class Fonts {
public:
    static void add(const FontConfiguration &config, bool isDefault = false);
    static void pushFont(const std::string &fontName);
    static void popFont();
    static ImFont *get(const std::string &fontName);
};

} // namespace Nest