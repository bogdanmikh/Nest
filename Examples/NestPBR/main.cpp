#include <Nest.hpp>

#include "src/NestPBR.hpp"

int main() {
    auto *application = Application::getInstance();
    auto *layer = new Profiler();
    application->setLayer(layer);
    application->loop();
    layer->detach();
    delete layer;
    delete application;
}
