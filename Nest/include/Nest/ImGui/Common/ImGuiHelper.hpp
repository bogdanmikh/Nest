//
// Created by Michael Andreichev on 20.12.2023.
//

#pragma once

#include <Panda/ImGui/Colors.hpp>
#include <Panda/ScriptEngine/ExternalScript.hpp>
#include <Panda/Base/Base.hpp>
#include <Panda/Assets/Texture.hpp>
#include <imgui_internal.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <string>

namespace Panda {

enum class VectorAxes { X = 1u << 0, Y = 1u << 1, Z = 1u << 2, W = 1u << 3 };

void shiftCursorX(float distance);
void shiftCursorY(float distance);
void shiftCursor(float x, float y);
void separator(ImVec2 size, ImVec4 color);
void beginDisabled(bool disabled = true);
bool isItemDisabled();
void endDisabled();
void underline(bool fullWidth = false, float offsetX = 0.0f, float offsetY = -1.0f);

bool dragFloat(
    const std::string &label,
    float *v,
    float v_speed = 1.0f,
    float v_min = 0.0f,
    float v_max = 0.0f,
    const char *format = "%.3f",
    ImGuiSliderFlags flags = 0,
    bool isInconsistent = false
);
bool dragFloat2(
    const std::string &label,
    float v[2],
    float v_speed = 1.0f,
    float v_min = 0.0f,
    float v_max = 0.0f,
    const char *format = "%.3f",
    ImGuiSliderFlags flags = 0,
    bool isInconsistent = false
);
bool dragFloat3(
    const std::string &label,
    float v[3],
    float v_speed = 1.0f,
    float v_min = 0.0f,
    float v_max = 0.0f,
    const char *format = "%.3f",
    ImGuiSliderFlags flags = 0,
    bool isInconsistent = false
);
bool dragFloat4(
    const std::string &label,
    float v[4],
    float v_speed = 1.0f,
    float v_min = 0.0f,
    float v_max = 0.0f,
    const char *format = "%.3f",
    ImGuiSliderFlags flags = 0,
    bool isInconsistent = false
);
bool dragInt(
    const std::string &label,
    int *v,
    float v_speed = 1.0f,
    int v_min = 0,
    int v_max = 0,
    const char *format = "%d",
    ImGuiSliderFlags flags = 0,
    bool isInconsistent = false
); // If v_min >= v_max we have no bound
bool dragInt2(
    const std::string &label,
    int v[2],
    float v_speed = 1.0f,
    int v_min = 0,
    int v_max = 0,
    const char *format = "%d",
    ImGuiSliderFlags flags = 0,
    bool isInconsistent = false
);
bool dragInt3(
    const std::string &label,
    int v[3],
    float v_speed = 1.0f,
    int v_min = 0,
    int v_max = 0,
    const char *format = "%d",
    ImGuiSliderFlags flags = 0,
    bool isInconsistent = false
);
bool dragInt4(
    const std::string &label,
    int v[4],
    float v_speed = 1.0f,
    int v_min = 0,
    int v_max = 0,
    const char *format = "%d",
    ImGuiSliderFlags flags = 0,
    bool isInconsistent = false
);
bool checkbox(const std::string &label, bool *v, bool isInconsistent);
int combo(
    const std::string &label, const std::vector<std::string> &list, int current, bool isInconsistent
);
bool drawVec3Control(
    const std::string &label,
    glm::vec3 &values,
    float resetValue = 0.f,
    uint32_t inconsistentAxes = 0
);
bool propertyColor(const char *label, Color &value, bool isInconsistent);
bool propertyTexture(
    const char *label, UUID &textureId, Foundation::Shared<Asset> asset, bool isInconsistent
);
bool drawScriptFieldValue(ScriptField &field);

} // namespace Panda