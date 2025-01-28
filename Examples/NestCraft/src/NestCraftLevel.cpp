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

static void drawCross();

void NestCraftLevel::onUpdate(double deltaTime) {
    Bird::setViewClear(0, 0x3D75C9FF);
    drawCross();
    m_viewport.update();
    for (const auto &entity : m_entities) {
        entity->onUpdate(deltaTime);
        entity->onImGuiRender();
    }
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

static void drawCross() {
    ImGuiIO &io = ImGui::GetIO();
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin(
        "##crosshair",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground
    );
    //      ImGui::Begin("##crosshair");

    ImDrawList *drawList = ImGui::GetWindowDrawList();

    float windowWidth = io.DisplaySize.x;
    float windowHeight = io.DisplaySize.y;

    float centerX = windowWidth / 2.0f;
    float centerY = windowHeight / 2.0f;

    float lineLength = 20.0f;

    auto color = IM_COL32(255, 0, 0, 255);
    drawList->AddLine(
        ImVec2(centerX - lineLength, centerY), ImVec2(centerX + lineLength, centerY), color
    );
    drawList->AddLine(
        ImVec2(centerX, centerY - lineLength), ImVec2(centerX, centerY + lineLength), color
    );
    ImGui::End();
}
