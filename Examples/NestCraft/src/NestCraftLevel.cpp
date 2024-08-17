#include <iostream>
#include "NestCraftLevel.hpp"
#include "CoreGame/ChunksRenderer.hpp"
#include "CoreGame/Menu.hpp"
#include "CameraMove.hpp"
#include "CoreGame/Cross.hpp"
#include <filesystem>

void NestCraftLevel::onAttach() {
    auto cameraMove = NEW(Foundation::getAllocator(), CameraMove);
    addEntity(cameraMove);

    auto chunksRenderer = NEW(Foundation::getAllocator(), ChunksRenderer);
    addEntity(chunksRenderer);
    //    auto *cross = new Cross;
    //    cross->init();
    //    NestCraftLevelObjects.emplace_back(cross);
}

void NestCraftLevel::onUpdate(double deltaTime) {
    for (const auto &entity : m_entities) {
        entity->onUpdate(deltaTime);
        entity->onImGuiRender();
    }
}

void NestCraftLevel::onDetach() {
    for (auto &entity : m_entities) {
        entity->onDetach();
        FREE(Foundation::getAllocator(), entity);
    }
}

void NestCraftLevel::addEntity(Nest::Entity *entity) {
    entity->onAttach();
    m_entities.emplace_back(entity);
}
