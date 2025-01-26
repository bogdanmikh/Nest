#include "Nest/ImGui/ImGuiFonts.hpp"
#include "Nest/Assets/AssetLoader.hpp"
#include "Foundation/Assert.hpp"
#include "Foundation/PlatformDetection.hpp"
#include <filesystem>

namespace Nest {

static std::unordered_map<std::string, ImFont *> s_fonts;

void Fonts::add(const FontConfiguration &config, bool isDefault) {
    NEST_ASSERT_F(
        s_fonts.find(config.fontName) == s_fonts.end(),
        "Tried to add font with name '{}' but that name is already taken!",
        config.fontName
    );
    ImFontConfig imguiFontConfig;
    imguiFontConfig.MergeMode = config.mergeWithLast;
    imguiFontConfig.OversampleH = 4;
    imguiFontConfig.OversampleV = 4;
    auto &io = ImGui::GetIO();
    ImFont *font;
    std::filesystem::path fontPath = "default-fonts/" + config.fileName;
#ifdef PLATFORM_DESKTOP
    font = io.Fonts->AddFontFromFileTTF(
        fontPath.string().c_str(),
        config.size,
        &imguiFontConfig,
        config.glyphRanges == nullptr ? io.Fonts->GetGlyphRangesDefault() : config.glyphRanges
    );
    NEST_ASSERT(font != nullptr, "Failed to load font file!");
#elif defined(PLATFORM_ANDROID)
    auto fontData = AssetLoader::readFile(fontPath);
    NEST_ASSERT(fontData.has_value(), "Failed to load file!");
    font = io.Fonts->AddFontFromMemoryTTF(
        fontData.value().first.data,
        fontData.value().second,
        config.size,
        &imguiFontConfig,
        config.glyphRanges == nullptr ? io.Fonts->GetGlyphRangesDefault() : config.glyphRanges
    );
    NEST_ASSERT(font != nullptr, "Failed to load font file!");
#endif
    s_fonts[config.fontName] = font;
    if (isDefault) {
        io.FontDefault = font;
    }
}

ImFont *Fonts::get(const std::string &fontName) {
    NEST_ASSERT(s_fonts.find(fontName) != s_fonts.end(), "Failed to find font with that name!");
    return s_fonts.at(fontName);
}

void Fonts::pushFont(const std::string &fontName) {
    const auto &io = ImGui::GetIO();
    if (s_fonts.find(fontName) == s_fonts.end()) {
        ImGui::PushFont(io.FontDefault);
        return;
    }
    ImGui::PushFont(s_fonts.at(fontName));
}

void Fonts::popFont() {
    ImGui::PopFont();
}

} // namespace Nest