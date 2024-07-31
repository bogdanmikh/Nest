#pragma once

#include <Nest.hpp>
#include "CoreGame/GameObject.hpp"
#include "CoreGame/Menu.hpp"

class Game final : public Layer {
public:
    void start() override;
    void update(double deltaTime) override;
    void detach() override;

private:
    Shader *shader;
    Menu menu;
    std::vector<GameObject *> gameObjects;
};
