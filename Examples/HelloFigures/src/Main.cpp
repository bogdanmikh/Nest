#include "FiguresLevel.hpp"

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "Hello figures";
    settings.windowTitle = "Hello figures";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = NEW(Foundation::getAllocator(), Nest::Application(settings));
    application->setLayer(NEW(Foundation::getAllocator(), FigureLevel));
    application->loop();
    DELETE(Foundation::getAllocator(), application);
    return 0;
}
