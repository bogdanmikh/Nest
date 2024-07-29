#include <iostream>
#include "Game.hpp"
#include "CoreGame/ChunksRenderer.hpp"
#include "CoreGame/Menu.hpp"
#include "CameraMove.hpp"
#include "CoreGame/Cross.hpp"
#include <filesystem>

void Game::start() {
    shader = new Shader("Shaders/vst.glsl",
                              "Shaders/fst.glsl");
    gameObjects.emplace_back(new CameraMove);
    auto *chunksRenderer = new ChunksRenderer;
    chunksRenderer->init();
    gameObjects.emplace_back(chunksRenderer);
//    auto *cross = new Cross;
//    cross->init();
//    gameObjects.emplace_back(cross);
}

void Game::update(double deltaTime) {
    auto camera = Application::getInstance()->getCamera();
    shader->use();
    shader->setFloat("u_time", Application::getInstance()->getWindow()->getTime());
    shader->setVec2("u_mouse", Events::getCursorPos());
    shader->setVec2("u_resolution", Application::getInstance()->getWindow()->getSize());
    shader->setMat4("u_model", glm::mat4(1));
    shader->setMat4("u_view", camera->getViewMatrix());
    shader->setMat4("u_projection", camera->getProjectionMatrix());
    shader->setVec3("u_color", menu.getColor());
    menu.update(deltaTime);
    for (const auto &item: gameObjects) {
        item->update(deltaTime);
    }
}

void Game::detach() {
    for (const auto &item: gameObjects) {
        delete item;
    }
    delete shader;
}

