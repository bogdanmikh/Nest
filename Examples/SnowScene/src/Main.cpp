#include "SnowSceneLevel.hpp"

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "Hello triangle";
    settings.windowTitle = "Snow scene";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = new Nest::Application(settings);
    application->setLayer(new SnowSceneLevel);
    application->loop();
    delete application;
    return 0;
}
