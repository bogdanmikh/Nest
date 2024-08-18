#include "FiguresLevel.hpp"

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "Hello triangle";
    settings.windowTitle = "Hello triangle";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = new Nest::Application(settings);
    application->setLayer(NEW(Foundation::getAllocator(), FigureLevel));
    application->loop();
    DELETE(Foundation::getAllocator(), application);
    return 0;
}
