//
// Created by admin on 26.12.2023.
//

#include <iostream>
#include <NestUI.hpp>
#include "Game.hpp"
#include "Chunk.hpp"
#include "ChunkMeshGenerator.hpp"

void Game::start(Window *window) {
    auto *shader = new Shader("/home/bogdan/Projects/Nest/Nest/res/Shaders/vst.glsl",
                              "/home/bogdan/Projects/Nest/Nest/res/Shaders/fst.glsl");

    auto *chunk1 = new Chunk(20, 20, 20);

    ChunkMeshGenerator chunkMeshGenerator;
    Mesh* mesh = chunkMeshGenerator.generateMesh(chunk1, 0, 0, 0);
    mesh->addTexture("/home/bogdan/Projects/Nest/Examples/NestCraft/res/textures/BlocksTile.png");

    Camera camera;
    camera.setShader(shader);
    camera.setPosition(0.f, 0.f, 5.f);

    float cameraSpeed = 5.f;
    Renderer::init();

    float lastTime = window->getTime();
    int frames = 0;
    const int maxFrames = 60;
    double sec = 0;
    window->toggleCursorLock();
    glm::vec2 lastPos = window->getCursorPos();

    while (!window->shouldClose() && !window->isKeyPressed(Key::ESCAPE)) {
        float currentTime = window->getTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        sec += deltaTime;
        if (sec > 1.) {
            std::cout << "FPS: " << frames << std::endl;
            sec = 0;
            frames = 0;
        }
        if (frames >= maxFrames && sec < 1.) {
            continue;
        }
        frames++;

        Renderer::clear();

        if (window->isKeyPressed(Key::W)) {
            camera.translateLocal(0., 0., cameraSpeed * deltaTime);
        }
        if (window->isKeyPressed(Key::S)) {
            camera.translateLocal(0., 0., -cameraSpeed * deltaTime);
        }
        if (window->isKeyPressed(Key::A)) {
            camera.translateLocal(-cameraSpeed * deltaTime, 0., 0.);
        }
        if (window->isKeyPressed(Key::D)) {
            camera.translateLocal(cameraSpeed * deltaTime, 0., 0.);
        }
        if (window->isKeyPressed(Key::SPACE)) {
            camera.translateLocal( 0., cameraSpeed * deltaTime, 0.);
        }
        if (window->isKeyPressed(Key::LEFT_CONTROL)) {
            camera.translateLocal( 0., -cameraSpeed * deltaTime, 0.);
        }

        glm::vec2 cursorPos = window->getCursorPos();
        glm::vec2 diff = lastPos - cursorPos;
        lastPos = cursorPos;
        float mouseSpeed = 0.1f;
        camera.rotate(-diff.y * mouseSpeed, -diff.x * mouseSpeed, 0.f);

        glm::vec2 resolution = window->getSize();
        camera.updateAspectRatio(resolution.x / resolution.y);
        Renderer::setRenderBufferSize(resolution.x, resolution.y);

        shader->setFloat("u_time", window->getTime());
        shader->setVec2("u_mouse", window->getCursorPos());
        shader->setVec2("u_resolution", window->getSize());
        shader->setMat4("u_model", glm::mat4(1));
        Renderer::checkForErrors();

        mesh->draw();

        window->swapBuffers();

        window->pollEvents();
        Renderer::checkForErrors();
    }
    delete mesh;
    delete chunk1;
    delete shader;
}