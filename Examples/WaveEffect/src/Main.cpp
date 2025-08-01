#include "WaveEffectLevel.hpp"

int startApp(int argc, char **argv) {
    Nest::ApplicationStartupSettings settings;
    settings.name = "Wave Effect";
    settings.windowTitle = "Wave Effect";
    settings.windowSize = {900, 600};
    settings.isFullScreen = false;
    auto application = new Nest::Application(settings);
    application->setLayer(new WaveEffectLevel);
    application->loop();
    delete application;
    return 0;
}
