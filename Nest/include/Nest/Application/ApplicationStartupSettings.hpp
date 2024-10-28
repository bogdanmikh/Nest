//
// Created by Bogdan
//

#pragma once

#include "Nest/Base/Base.hpp"
namespace Nest {

struct ApplicationStartupSettings {
public:
    enum Platform { DESKTOP, PHONE };
    const char *name;
    const char *windowTitle;
    Size windowSize;
    bool isFullScreen;
    bool isMaximized;
    Platform platform;

    ApplicationStartupSettings()
        : name("Nest Sandbox App")
        , windowTitle("Nest")
        , windowSize(Size(600, 400))
        , isFullScreen(false)
        , isMaximized(false)
        , platform(Platform::DESKTOP) {}

    ApplicationStartupSettings(
        const char *name,
        const char *windowTitle,
        const Size &windowSize,
        bool isFullScreen,
        bool isMaximized
    )
        : name(name)
        , windowTitle(windowTitle)
        , windowSize(windowSize)
        , isFullScreen(isFullScreen)
        , isMaximized(isMaximized) {}
};

} // namespace Nest