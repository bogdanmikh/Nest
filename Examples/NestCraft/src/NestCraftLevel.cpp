#include "NestCraftLevel.hpp"
#include "CameraMove.hpp"
#include "CoreGame/ChunksRenderer.hpp"
#include "CoreGame/Cross.hpp"
#include "CoreGame/Menu.hpp"
#include "Effects/ManagerEffects.hpp"
#include <iostream>

void NestCraftLevel::onAttach() {
    m_viewport.init();

    auto cameraMove = F_NEW(Foundation::getAllocator(), CameraMove);
    addEntity(cameraMove);

    auto chunksRenderer = F_NEW(Foundation::getAllocator(), ChunksRenderer);
    chunksRenderer->setFbViewId(m_viewport.getViewId());
    //    chunksRenderer->setFbViewId(0);
    addEntity(chunksRenderer);

    auto managerEffects = F_NEW(Foundation::getAllocator(), ManagerEffects);
    managerEffects->setFBTexture(m_viewport.getTextureHandle());
    managerEffects->setViewId(m_viewport.getViewId());
    addEntity(managerEffects);
}

void NestCraftLevel::onUpdate(double deltaTime) {
    Bird::setViewClear(0, 0x3D75C9FF);
    m_viewport.update();
    for (const auto &entity : m_entities) {
        entity->onUpdate(deltaTime);
        entity->onImGuiRender();
    }
    //    drawCross();
}

void NestCraftLevel::onDetach() {
    for (auto &entity : m_entities) {
        entity->onDetach();
        F_DELETE(Foundation::getAllocator(), entity);
    }
}

void NestCraftLevel::addEntity(Nest::Entity *entity) {
    entity->onAttach();
    m_entities.emplace_back(entity);
}