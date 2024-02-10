#include <iostream>
#include "Game.hpp"
#include "CoreGame/ChunksRenderer.hpp"
#include "CoreGame/Menu.hpp"
#include "CameraMove.hpp"
#include "CoreGame/Cross.hpp"

void Game::start() {
    shader = new Shader("Nest/res/Shaders/vst.glsl",
                              "Nest/res/Shaders/fst.glsl");
    Application::getInstance()->getCamera()->setShader(shader);
    gameObjects.emplace_back(new CameraMove);
    auto *chunksRenderer = new ChunksRenderer;
    chunksRenderer->init();
    gameObjects.emplace_back(chunksRenderer);
//    auto *cross = new Cross;
//    cross->init();
//    gameObjects.emplace_back(cross);
    gameObjects.emplace_back(new Menu);
}

void Game::update(double deltaTime) {
    shader->use();
    shader->setFloat("u_time", Application::getInstance()->getWindow()->getTime());
    shader->setVec2("u_mouse", Events::getCursorPos());
    shader->setVec2("u_resolution", Application::getInstance()->getWindow()->getSize());
    shader->setMat4("u_model", glm::mat4(1));
    for (const auto &item: gameObjects) {
        item->update(deltaTime);
    }
}

Game::~Game() {
    for (const auto &item: gameObjects) {
        delete item;
    }
    delete shader;
}
