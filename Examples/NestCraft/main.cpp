#include <Nest.hpp>

#include "src/Game.hpp"

int main() {
    auto *application = Application::getInstance();
    auto *layer = new Game();
    application->setLayer(layer);
    application->loop();
    layer->detach();
    delete layer;
    delete application;
}
