//
// Created by admin on 26.12.2023.
//

#include "Game.hpp"

class CubeGame final : public Cube {
public:
    CubeGame(Shader *shader) : Cube("/home/bogdan/Projects/Nest/Examples/HelloTexture/res/textures/Bohdan.png",
                                    shader) {}

    void update(double deltaTime) override {
        draw();
    }
};

void Game::start(Window *window) {
    World world;
    auto *shader = new Shader("/home/bogdan/Projects/Nest/Nest/res/Shaders/vst.glsl",
                              "/home/bogdan/Projects/Nest/Nest/res/Shaders/fst.glsl");
    auto *cubeGame = new CubeGame(shader);
    world.addActor(cubeGame);
    Renderer::init();

    while (!window->shouldClose() && !window->isKeyPressed(Key::ESCAPE)) {
        Renderer::clear();

        glm::vec3 position = cubeGame->getPosition();
        glm::vec3 rotation = cubeGame->getRotation();

        glm::vec2 resolution = window->getSize();
        Renderer::setRenderBufferSize(resolution.x, resolution.y);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), resolution.x / resolution.y,
                                                0.001f, 100.0f);

        glm::mat4 model(1.f);
        shader->use();
        if (window->isKeyPressed(Key::A)) {
            position.x += 0.1f;
        } else if (window->isKeyPressed(Key::D)) {
            position.x -= 0.1f;
        } else if (window->isKeyPressed(Key::W)) {
            position.z += 0.1f;
        } else if (window->isKeyPressed(Key::S)) {
            position.z -= 0.1f;
        } else if (window->isKeyPressed(Key::SPACE)) {
            position.y -= 0.1f;
        } else if (window->isKeyPressed(Key::LEFT_ALT)) {
            position.y += 0.1f;
        }

        rotation.x -= 0.01f;
        rotation.y -= 0.01f;
        model = glm::translate(model, position);

        model = glm::rotate(model, rotation.x, glm::vec3(1.f, 0.f, 0.f));
        model = glm::rotate(model, rotation.y, glm::vec3(0.f, 1.f, 0.f));
        model = glm::rotate(model, rotation.z, glm::vec3(0.f, 0.f, 1.f));

        shader->setFloat("u_time", window->getTime());
        shader->setVec2("u_mouse", window->getCursorPos());
        shader->setVec2("u_resolution", window->getSize());
        shader->setMat4("u_model", model);
        shader->setMat4("u_projection", projection);

        Renderer::drawIndexed(36);

        window->swapBuffers();

        window->pollEvents();
    }
    delete cubeGame;
    delete shader;
}
