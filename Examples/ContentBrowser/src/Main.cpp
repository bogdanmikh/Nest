#include "ContentBrowserLevel.hpp"

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "ContentBrowser";
    settings.windowTitle = "ContentBrowser";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = new Nest::Application(settings);
    application->setLayer(NEW(Foundation::getAllocator(), ContentBrowserLevel));
    application->loop();
    DELETE(Foundation::getAllocator(), application);
    return 0;
}
