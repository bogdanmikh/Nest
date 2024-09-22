#include "AtomsGameLevel.hpp"

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "Atoms";
    settings.windowTitle = "Atoms";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = new Nest::Application(settings);
    application->setLayer(new AtomsGameLevel);
    application->loop();
    delete application;
    return 0;
}
