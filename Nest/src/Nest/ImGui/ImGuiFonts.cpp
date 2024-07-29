#include "Nest/ImGui/ImGuiFonts.hpp"
#include "Nest/Logger/Logger.hpp"

void Fonts::add(const FontConfiguration &config, bool isDefault) {
    ImFontConfig imguiFontConfig;
    imguiFontConfig.MergeMode = config.mergeWithLast;
    imguiFontConfig.OversampleH = 4;
    imguiFontConfig.OversampleV = 4;
    auto &io = ImGui::GetIO();
    std::string fontPath = "Fonts/" + config.fileName;
    ImFont *font = io.Fonts->AddFontFromFileTTF(
        fontPath.c_str(),
        config.size,
        &imguiFontConfig,
        config.glyphRanges == nullptr ? io.Fonts->GetGlyphRangesDefault() : config.glyphRanges
    );
    // assert(font != nullptr);
    LOG_INFO("ADDED {} FONT", config.fontName);
    std::unordered_map<std::string, ImFont *> &fonts = getFonts();
    fonts[config.fontName] = font;
    if (isDefault) {
        io.FontDefault = font;
    }
}

ImFont *Fonts::get(const std::string &fontName) {
    std::unordered_map<std::string, ImFont *> &fonts = getFonts();
    assert(fonts.find(fontName) != fonts.end());
    return fonts.at(fontName);
}

void Fonts::pushFont(const std::string &fontName) {
    std::unordered_map<std::string, ImFont *> &fonts = getFonts();
    const auto &io = ImGui::GetIO();
    if (fonts.find(fontName) == fonts.end()) {
        ImGui::PushFont(io.FontDefault);
        return;
    }
    ImGui::PushFont(fonts.at(fontName));
}

void Fonts::popFont() {
    ImGui::PopFont();
}

std::unordered_map<std::string, ImFont *> &Fonts::getFonts() {
    static std::unordered_map<std::string, ImFont *> fonts;
    return fonts;
}