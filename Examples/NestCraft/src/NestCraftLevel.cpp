#include <iostream>
#include "NestCraftLevel.hpp"
#include "CoreGame/ChunksRenderer.hpp"
#include "CoreGame/Menu.hpp"
#include "CameraMove.hpp"
#include "CoreGame/Cross.hpp"
#include <filesystem>

void NestCraftLevel::onAttach() {
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vst.glsl", "Shaders/fst.glsl");
    m_shader = createProgram(programAsset.getBirdProgramCreate());
    auto cameraMove = NEW(Foundation::getAllocator(), CameraMove);
    addEntity(cameraMove);

    auto  chunksRenderer = new ChunksRenderer;
    chunksRenderer->onAttach();
    addEntity(chunksRenderer);
    //    auto *cross = new Cross;
    //    cross->init();
    //    NestCraftLevelObjects.emplace_back(cross);
}

void NestCraftLevel::onUpdate(double deltaTime) {
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
    for (const auto &item : NestCraftLevelObjects) {
        item->update(deltaTime);
    }
}

void NestCraftLevel::onDetach() {
    for (const auto &item : NestCraftLevelObjects) {
        delete item;
    }
    delete shader;
}

void NestCraftLevel::addEntity(Nest::Entity *entity) {
    m_entities.emplace_back(entity);
}
