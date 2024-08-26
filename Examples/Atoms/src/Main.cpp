#include "AtomsGameLevel.hpp"

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "Atoms";
    settings.windowTitle = "Atoms";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = NEW(Foundation::getAllocator(), Nest::Application(settings));
    application->setLayer(NEW(Foundation::getAllocator(), AtomsGameLevel));
    application->loop();
    DELETE(Foundation::getAllocator(), application);
    return 0;
}
