#pragma once

#include <Nest.hpp>

class Game final: public Level {
public:
    ~Game();
    void start() override;
    void update(double deltaTime) override;
private:
    Shader* shader;
    std::vector<GameObject*> gameObjects;
};

