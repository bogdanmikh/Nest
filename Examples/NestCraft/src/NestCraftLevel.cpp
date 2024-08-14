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

    auto chunksRenderer = new ChunksRenderer;
    chunksRenderer->onAttach();
    addEntity(chunksRenderer);
    //    auto *cross = new Cross;
    //    cross->init();
    //    NestCraftLevelObjects.emplace_back(cross);
}

void NestCraftLevel::onUpdate(double deltaTime) {
    for (const auto &item : m_entities) {
        item->onUpdate(deltaTime);
    }
}

void NestCraftLevel::onDetach() {
    for (const auto &item : m_entities) {
        item->onDetach();
        delete item;
    }
}

void NestCraftLevel::addEntity(Nest::Entity *entity) {
    m_entities.emplace_back(entity);
}
