//
// Created by admin on 26.12.2023.
//

#include <iostream>
#include <NestUI.hpp>
#include "Game.hpp"
#include "ChunkManager.hpp"
#include "ChunkMeshGenerator.hpp"

void Game::start(Window *window) {
    auto *shader = new Shader("/home/bogdan/Projects/NewNest/Nest/res/Shaders/vst.glsl",
                              "/home/bogdan/Projects/NewNest/Nest/res/Shaders/fst.glsl");
    ChunkMeshGenerator chunkMeshGenerator;
    auto* chunkManager = new ChunkManager(4, 4, 1);
    Mesh **meshes = new Mesh*[chunkManager->getSize()];
    int index = 0;
    for (int i = 0; i < chunkManager->getSize(); ++i) {
        std::cout << "Chunk " << index << " generic" << std::endl;
        Chunk *chunk = chunkManager->chunks[i];
        if (!chunk) {
            std::cout << "Error:Chunk IS NULL\n";
            continue;
        }
        Mesh *mesh = chunkMeshGenerator.generateMesh(chunk);
        mesh->addTexture("/home/bogdan/Projects/NewNest/Examples/NestCraft/res/textures/BlocksTile.png");
        meshes[index++] = mesh;
    }
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
        Renderer::checkForErrors();
        for (int i = 0; i < chunkManager->getSize(); ++i) {
            glm::mat4 model(1);
            std::cout << "Chunk " << i << " draw: ";
            Chunk *chunk = chunkManager->chunks[i];
            if (!chunk) {
                std::cout << "\nError:Chunk IS NULL\n";
                continue;
            }
            Mesh *mesh = meshes[i];
            glm::translate(model, glm::vec3(chunk->getPosX() * chunkManager->getSizeX(),
                                            chunk->getPosY() * chunkManager->getSizeY(),
                                            chunk->getPosZ() * chunkManager->getSizeZ()));
            shader->setMat4("u_model", model);
            mesh->draw();
            std::cout << chunk->getPosX() * chunkManager->getSizeX() << " " <<
                chunk->getPosY() * chunkManager->getSizeY() << " " <<
                chunk->getPosZ() * chunkManager->getSizeZ() << std::endl;
        }

        window->swapBuffers();

        window->pollEvents();
        Renderer::checkForErrors();
    }
    delete chunkManager;
    delete[] meshes;
    delete shader;
}