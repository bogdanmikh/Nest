#pragma once

#include "Nest.hpp"
#include <vector>

class World {
public:
    inline World() = default;
    void update(double deltaTime);
    void addActor(GameObject* actor);
    void deleteActor(GameObject* actor);
    void deleteAll();
private:
    std::vector<GameObject*> gameObjects;
};
