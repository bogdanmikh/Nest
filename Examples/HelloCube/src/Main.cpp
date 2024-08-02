#include "CubeLevel.hpp"

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "Hello triangle";
    settings.windowTitle = "Hello triangle";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = new Nest::Application(settings);
    application->setLayer(NEW(Foundation::getAllocator(), CubeLevel));
    application->loop();
    FREE(Foundation::getAllocator(), application);
}
