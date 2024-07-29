#pragma once

#include <imgui.h>
#include <unordered_map>
#include <string>

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
    static std::unordered_map<std::string, ImFont *> &getFonts();

private:
};