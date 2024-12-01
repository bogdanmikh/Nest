#include "ProfilerTest.hpp"
#include <Nest.hpp>

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "NestPBR";
    settings.windowTitle = "NestPBR";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = new Nest::Application(settings);
    application->setLayer(new ProfilerTest);
    application->loop();
    delete application;
    return 0;
}