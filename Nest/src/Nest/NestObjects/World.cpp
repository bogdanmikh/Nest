#include <algorithm>

#include "Nest/NestObjects/World.hpp"

void World::update(double deltaTime) {
    for (auto & gameObject : gameObjects) {
        gameObject->draw();
    }
}

void World::addActor(GameObject* actor) {
    gameObjects.push_back(actor);
}

void World::deleteActor(GameObject* actor) {
    auto it = std::find(gameObjects.begin(), gameObjects.end(), actor);
    if(it != gameObjects.end()) {
        gameObjects.erase(it);
        delete actor;
    }
}

void World::deleteAll() {
    for(auto& actor: gameObjects) {
        delete actor;
    }
    gameObjects.clear();
}
