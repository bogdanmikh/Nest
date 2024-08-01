#pragma once

namespace Nest {

class ImGuiLayer {
public:
    ImGuiLayer() {}
    ~ImGuiLayer() = default;

    void onAttach();
    void onDetach();
    void setBlockEvents(bool block) {
        m_blockEvents = block;
    }

    void begin(double deltaTime);
    void end();

    void setDarkThemeColors();

private:
    bool m_blockEvents = true;
};

} // namespace Nest