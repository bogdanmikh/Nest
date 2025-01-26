#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Nest/Window/Events.hpp"

namespace Nest {

class AndroidEvents final : public Events {
public:
    void init(void *handle) override;
    glm::vec2 getCursorPos() override;
    double getTime() override;
    bool isKeyPressed(Key key) override;
    bool isJustKeyPressed(Key key) override;
    bool isMouseButtonPressed(MouseButton mouseButton) override;
    bool isJustMouseButtonPressed(MouseButton mouseButton) override;
    void toggleCursorLock() override;
    bool isCursorLocked() override;
    void pollEvents() override;
    std::vector<std::string> getDropPaths() override;
    void resetDropPaths() override;
};

} // namespace Nest