#include "NestCraftLevel.hpp"

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "NestCraft";
    settings.windowTitle = "NestCraft";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = NEW(Foundation::getAllocator(), Nest::Application(settings));
    application->setLayer(NEW(Foundation::getAllocator(), NestCraftLevel));
    application->loop();
    DELETE(Foundation::getAllocator(), application);
    return 0;
}
