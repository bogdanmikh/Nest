#include "HelloLightLevel.hpp"

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "Hello Light";
    settings.windowTitle = "Hello Light";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = new Nest::Application(settings);
    application->setLayer(new HelloLightLevel);
    application->loop();
    delete application;
    return 0;
}
