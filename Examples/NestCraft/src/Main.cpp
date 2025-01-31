#include "NestCraftLevel.hpp"

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "NestCraft";
    settings.windowTitle = "NestCraft";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = new Nest::Application(settings);
    application->setLayer(new NestCraftLevel);
    application->loop();
    delete application;
    return 0;
}
