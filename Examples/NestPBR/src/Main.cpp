#include "TriangleLevel.hpp"

#include <Nest.hpp>

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "Hello triangle";
    settings.windowTitle = "Hello triangle";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = new Nest::Application(settings);
    application->setLayer();
    application->loop();
    delete application;
    return 0;
}