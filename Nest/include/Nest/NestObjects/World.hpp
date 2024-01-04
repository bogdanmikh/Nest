#pragma once

#include <Nest.hpp>
#include <vector>

class World {
public:
    inline World() {}
    void update(double deltaTime);
    void addActor(GameObject* actor);
    void deleteActor(GameObject* actor);
    void deleteAll();
private:
    VertexArray *vertexArray;
    std::vector<GameObject*> gameObjects;
};
