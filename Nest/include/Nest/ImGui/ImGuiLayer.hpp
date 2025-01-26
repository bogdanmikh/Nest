#pragma once

#include "Nest/Application/Layer.hpp"

namespace Nest {

class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer() = default;

    void onAttach() override;
    void onDetach() override;
    void onEvent(Event *event) override;

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