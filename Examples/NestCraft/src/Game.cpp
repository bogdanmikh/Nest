#include <iostream>
#include "Game.hpp"
#include "CoreGame/ChunksRenderer.hpp"
#include "CoreGame/Menu.hpp"
#include "CameraMove.hpp"
#include "CoreGame/Cross.hpp"
#include <filesystem>

void Game::start() {
    shader = new Shader("Shaders/vst.glsl", "Shaders/fst.glsl");
    gameObjects.emplace_back(new CameraMove);
    auto *chunksRenderer = new ChunksRenderer;
    chunksRenderer->init();
    gameObjects.emplace_back(chunksRenderer);
    //    auto *cross = new Cross;
    //    cross->init();
    //    gameObjects.emplace_back(cross);
}

void drawCross() {
    int width, height;
    width = Application::getInstance()->getWindow()->getSize().x;
    height = Application::getInstance()->getWindow()->getSize().y;

    ImGui::SetNextWindowPos(ImVec2(width / 2, height / 2), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin("##crosshair");
//    ImGui::Begin("##crosshair", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    drawList->AddLine(ImVec2(p.x - 10, p.y), ImVec2(p.x + 10, p.y), IM_COL32(0, 0, 0, 255), 2.0f);
    drawList->AddLine(ImVec2(p.x, p.y - 10), ImVec2(p.x, p.y + 10), IM_COL32(0, 0, 0, 255), 2.0f);
    ImGui::End();
}

void DrawCross() {
    ImGui::Begin("##crosshair", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
//    ImGui::Begin("##crosshair");

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    ImGuiIO& io = ImGui::GetIO();
    float windowWidth = io.DisplaySize.x;
    float windowHeight = io.DisplaySize.y;

    float centerX = windowWidth / 2.0f;
    float centerY = windowHeight / 2.0f;

    float lineLength = 20.0f;

    auto color = IM_COL32(255, 0, 0, 255);
    drawList->AddLine(ImVec2(centerX - lineLength, centerY), ImVec2(centerX + lineLength, centerY), color);
    drawList->AddLine(ImVec2(centerX, centerY - lineLength), ImVec2(centerX, centerY + lineLength), color);
    ImGui::End();
}

void Game::update(double deltaTime) {
    DrawCross();
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
    for (const auto &item : gameObjects) {
        item->update(deltaTime);
    }
}

void Game::detach() {
    for (const auto &item : gameObjects) {
        delete item;
    }
    delete shader;
}
