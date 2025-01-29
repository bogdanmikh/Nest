//
// Created by Admin on 22.01.2022.
//

#include "PlatformInit.hpp"

#include <Foundation/Allocator.hpp>
#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_DESKTOP
#    include "Platform/WindowImpl/GlfwWindow/GlfwWindow.hpp"
#elif defined(PLATFORM_ANDROID)
#    include "Platform/WindowImpl/AndroidWindow/AndroidWindow.hpp"
#endif

namespace Nest {

Window *createWindow(ApplicationStartupSettings &settings) {
#ifdef PLATFORM_DESKTOP
    return F_NEW(Foundation::getAllocator(), GlfwWindow)(
        settings.windowTitle, settings.windowSize, settings.isFullScreen, settings.isMaximized
    );
#elif defined(PLATFORM_IOS)
    return F_NEW(Foundation::getAllocator(), PandaWindowIOSImpl);
#else
    return F_NEW(Foundation::getAllocator(), AndroidWindow)(
        settings.windowTitle, settings.windowSize, settings.isFullScreen, settings.isMaximized
    );
#endif
}

} // namespace Nest
