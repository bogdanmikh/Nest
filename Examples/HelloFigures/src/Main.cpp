#include "FiguresLevel.hpp"

#include <Foundation/PlatformDetection.hpp>
#ifdef PLATFORM_ANDROID
#    include <game-activity/GameActivity.cpp>
#    include <game-text-input/gametextinput.cpp>
#endif

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "Hello figures";
    settings.windowTitle = "Hello figures";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = new Nest::Application(settings);
    application->setLayer(new FigureLevel);
    application->loop();
    delete application;
    return 0;
}
