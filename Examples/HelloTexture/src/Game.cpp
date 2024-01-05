//
// Created by admin on 26.12.2023.
//

#include "Game.hpp"

void Game::start(Window *window) {
    auto *shader = new Shader("/home/bogdan/Projects/Nest/Nest/res/Shaders/vst.glsl",
                              "/home/bogdan/Projects/Nest/Nest/res/Shaders/fst.glsl");
    Renderer::checkForErrors();
    auto *cube = new Cube("/home/bogdan/Projects/Nest/Examples/HelloTexture/res/textures/Bohdan.png", shader);
    Renderer::init();
    Renderer::checkForErrors();

    while (!window->shouldClose() && !window->isKeyPressed(Key::ESCAPE)) {
        Renderer::clear();
        Renderer::checkForErrors();

        glm::vec3 position = cube->getPosition();
        glm::vec3 rotation = cube->getRotation();

        if (window->isKeyPressed(Key::W)) {
            position.z += 0.1f;
        } else if (window->isKeyPressed(Key::S)) {
            position.z -= 0.1f;
        } else if (window->isKeyPressed(Key::SPACE)) {
            position.y += 0.1f;
        }

         glm::vec2 resolution = window->getSize();
         Renderer::setRenderBufferSize(resolution.x, resolution.y);
         Renderer::checkForErrors();


        glm::mat4 projection = glm::perspective(glm::radians(45.0f), resolution.x / resolution.y,
                                                0.001f, 100.0f);
        Renderer::checkForErrors();

        rotation.x += 0.01f;
        rotation.y += 0.01f;

        Renderer::checkForErrors();
        shader->setFloat("u_time", window->getTime());
        Renderer::checkForErrors();
        shader->setVec2("u_mouse", window->getCursorPos());
        Renderer::checkForErrors();
        shader->setVec2("u_resolution", window->getSize());
        Renderer::checkForErrors();
        shader->setMat4("u_projection", projection);
        Renderer::checkForErrors();

        cube->draw();

        cube->setPosition(position);
        cube->setRotation(rotation);

        window->swapBuffers();
        Renderer::checkForErrors();

        window->pollEvents();
        Renderer::checkForErrors();
    }
    delete cube;
    delete shader;
}
