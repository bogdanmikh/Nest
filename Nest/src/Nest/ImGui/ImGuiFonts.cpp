#include "Panda/ImGui/ImGuiFonts.hpp"
#include "Panda/Assets/AssetLoader.hpp"

namespace Panda {

static std::unordered_map<std::string, ImFont *> s_fonts;

void Fonts::add(const FontConfiguration &config, bool isDefault) {
    PND_ASSERT_F(
        s_fonts.find(config.fontName) == s_fonts.end(),
        "Tried to add font with name '{}' but that name is already taken!",
        config.fontName
    );
    ImFontConfig imguiFontConfig;
    imguiFontConfig.MergeMode = config.mergeWithLast;
    imguiFontConfig.OversampleH = 4;
    imguiFontConfig.OversampleV = 4;
    auto &io = ImGui::GetIO();
    std::string fontPath = AssetLoader::getResourcesPath() + "default-fonts/" + config.fileName;
    ImFont *font = io.Fonts->AddFontFromFileTTF(
        fontPath.c_str(),
        config.size,
        &imguiFontConfig,
        config.glyphRanges == nullptr ? io.Fonts->GetGlyphRangesDefault() : config.glyphRanges
    );
    PND_ASSERT(font != nullptr, "Failed to load font file!");
    s_fonts[config.fontName] = font;
    if (isDefault) {
        io.FontDefault = font;
    }
}

ImFont *Fonts::get(const std::string &fontName) {
    PND_ASSERT(s_fonts.find(fontName) != s_fonts.end(), "Failed to find font with that name!");
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

} // namespace Panda