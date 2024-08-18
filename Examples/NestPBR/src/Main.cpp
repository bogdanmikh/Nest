#include "NestPBR.hpp"

#include <Nest.hpp>

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "NestPBR";
    settings.windowTitle = "NestPBR";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = NEW(Foundation::getAllocator(), Nest::Application(settings));
    application->setLayer(NEW(Foundation::getAllocator(), NestPBR));
    application->loop();
    DELETE(Foundation::getAllocator(), application);
    return 0;
}