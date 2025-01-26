//
// Created by Bogdan on 06.08.2024.
//

#include "SnowScene.hpp"
#include "Nest/Assets/AssetLoader.hpp"
#include "Rain/Coders/YamlDecoder.hpp"
#include "Rain/Coders/YamlEncoder.hpp"
#include "imgui_internal.h"

#include <fstream>

#define EDITOR_MODE 1

SnowScene::SnowScene() {}

void SnowScene::onAttach() {
    Nest::ProgramAsset programAsset = Nest::AssetLoader::loadProgram(
        "Shaders/Model3d_vertex.glsl", "Shaders/Model3d_fragment.glsl"
    );
    Bird::ProgramHandle shaderHandle = createProgram(programAsset.getBirdProgramCreate());
    m_infoObjects.emplace_back("Snow Car 1", "Models/snow_car/scene.gltf");
    m_infoObjects.emplace_back("Snow Car 2", "Models/snow_car2/scene.gltf");
    m_infoObjects.emplace_back("House", "Models/winter_house/scene.gltf");
    m_infoObjects.emplace_back("Fence 1", "Models/fence/scene.gltf");
    m_infoObjects.emplace_back("Fence 2", "Models/fence/scene.gltf");
    m_infoObjects.emplace_back("Place", "Models/place/scene.gltf");
    m_infoObjects.emplace_back("Firewood", "Models/firewood/scene.gltf");
    m_infoObjects.emplace_back("Place 2", "Models/place2/scene.gltf");
    m_infoObjects.emplace_back("Snowman", "Models/snowman/scene.gltf");

    m_objects.resize(m_infoObjects.size());
    m_objectsSettings.resize(m_infoObjects.size());
    for (int i = 0; i < m_infoObjects.size(); i++) {
        auto info = m_infoObjects[i];
        m_objectsSettings[i].name = info.name;
        m_objects[i].create(shaderHandle, info.pathToModel);
    }

    // Load general settings
    {
        std::ifstream file("Scenes/scene.yaml");
        if (file.is_open()) {
            Rain::Decoder *decoder = new Rain::YamlDecoder;
            decoder->decode(file, m_objectsSettings);
            delete decoder;
            file.close();
            LOG_INFO("LOAD");
        }
    }
    for (int i = 0; i < m_objects.size(); i++) {
        auto &obj = m_objects[i];
    }
}

void SnowScene::onUpdate(double deltaTime) {
    // house
    for (int i = 0; i < m_objects.size(); i++) {
        auto &obj = m_objects[i];
    }
    for (int i = 0; i < m_objects.size(); i++) {
        auto &obj = m_objects[i];
        auto &settings = m_objectsSettings[i];
        auto &transform = obj.getTransform();

        transform.setRotationEuler(settings.degrees);
        transform.setPosition(settings.position);
        transform.setScale(settings.scale);
        obj.draw();
    }
}

static bool drawVec3Control(const std::string &label, Nest::Vec3 &values, float resetValue);

void SnowScene::onImGuiRender() {
#if EDITOR_MODE
    for (int i = 0; i < m_objects.size(); i++) {
        auto &obj = m_objects[i];
        auto &settings = m_objectsSettings[i];
        drawVec3Control(settings.name + " Pos: ", m_objectsSettings[i].position, 0);
        drawVec3Control(settings.name + " Scale: ", m_objectsSettings[i].scale, 1);
        drawVec3Control(settings.name + " Deg: ", m_objectsSettings[i].degrees, 0);
    }
#endif
}

void SnowScene::onDetach() {
#if EDITOR_MODE
//    std::ofstream file("Scenes/scene.yaml");
//    if (file.is_open()) {
//        Rain::Encoder *encoder = new Rain::YamlEncoder();
//        encoder->encode(file, m_objectsSettings);
//        delete encoder;
//        file.close();
//        LOG_INFO("SAVE");
//    }
#endif
}

static bool drawVec3Control(const std::string &label, Nest::Vec3 &values, float resetValue) {
    ImGuiIO &io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];
    bool edited = false;

    ImGui::PushID(label.c_str());
    ImGui::Columns(2, nullptr, false);
    //   ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();
    ImGui::Spacing();

    const float spacingX = 0.0f;
    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    const ImVec2 buttonSize(lineHeight + 3.0f, lineHeight);
    const float inputItemWidth =
        (ImGui::GetContentRegionAvail().x - spacingX) / 3.0f - buttonSize.x;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize)) {
        values.x = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputItemWidth);
    edited = ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f") || edited;
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)) {
        values.y = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputItemWidth);
    edited = ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f") || edited;
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize)) {
        values.z = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputItemWidth);
    edited = ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f") || edited;

    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
    return edited;
}
