#include "ImGuiHelper.hpp"
#include "Model/DragDropItem.hpp"
#include "ProjectLoader/AssetHandlerEditor.hpp"

#include <Panda/GameLogic/GameContext.hpp>
#include <Panda/ImGui/FontAwesome.h>
#include <Panda/ImGui/ImGui+DragScalar.hpp>
#include <string>

namespace Panda {

static float firstColumnWidth = 120.f;
static float coefficientRounding = 3.0f;

void shiftCursorX(float distance) {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + distance);
}

void shiftCursorY(float distance) {
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + distance);
}

void shiftCursor(float x, float y) {
    const ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(cursor.x + x, cursor.y + y));
}

void separator(ImVec2 size, ImVec4 color) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, color);
    ImGui::BeginChild("sep", size);
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void beginDisabled(bool disabled) {
    if (disabled) {
        ImGui::BeginDisabled(true);
    }
}

bool isItemDisabled() {
    return ImGui::GetItemFlags() & ImGuiItemFlags_Disabled;
}

void endDisabled() {
    if (GImGui->DisabledStackSize > 0) {
        ImGui::EndDisabled();
    }
}

void underline(bool fullWidth, float offsetX, float offsetY) {
    if (fullWidth) {
        if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
            ImGui::PushColumnsBackground();
        else if (ImGui::GetCurrentTable() != nullptr)
            ImGui::TablePushBackgroundChannel();
    }
    const float width = fullWidth ? ImGui::GetWindowWidth() : ImGui::GetContentRegionAvail().x;
    const ImVec2 cursor = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddLine(
        ImVec2(cursor.x + offsetX, cursor.y + offsetY),
        ImVec2(cursor.x + width, cursor.y + offsetY),
        Theme::backgroundDark,
        1.0f
    );
    if (fullWidth) {
        if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
            ImGui::PopColumnsBackground();
        else if (ImGui::GetCurrentTable() != nullptr)
            ImGui::TablePopBackgroundChannel();
    }
}

bool dragFloat(
    const std::string &label,
    float *v,
    float v_speed,
    float v_min,
    float v_max,
    const char *format,
    ImGuiSliderFlags flags,
    bool isInconsistent
) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));

    ImGui::PushID(label.c_str());
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text(isInconsistent ? "*%s" : "%s", label.c_str());
    ImGui::NextColumn();
    ImGui::Spacing();

    ImGui::PushItemWidth(-1);
    ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isInconsistent);
    bool result =
        ImGui::DragScalarCustom("", ImGuiDataType_Float, v, v_speed, &v_min, &v_max, format, flags);
    ImGui::PopItemFlag();
    ImGui::PopItemWidth();
    ImGui::Columns(1);
    ImGui::PopID();
    ImGui::PopStyleVar(2);
    return result;
}

bool dragFloat2(
    const std::string &label,
    float v[2],
    float v_speed,
    float v_min,
    float v_max,
    const char *format,
    ImGuiSliderFlags flags,
    bool isInconsistent
) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));

    ImGui::PushID(label.c_str());
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text(isInconsistent ? "*%s" : "%s", label.c_str());
    ImGui::NextColumn();
    ImGui::Spacing();

    ImGui::PushItemWidth(-1);
    ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isInconsistent);
    bool result = ImGui::DragScalarCustomN(
        "", ImGuiDataType_Float, v, 2, v_speed, &v_min, &v_max, format, flags
    );
    ImGui::PopItemFlag();
    ImGui::PopItemWidth();
    ImGui::Columns(1);
    ImGui::PopID();
    ImGui::PopStyleVar(2);
    return result;
}

bool dragFloat3(
    const std::string &label,
    float v[3],
    float v_speed,
    float v_min,
    float v_max,
    const char *format,
    ImGuiSliderFlags flags,
    bool isInconsistent
) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));

    ImGui::PushID(label.c_str());
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text(isInconsistent ? "*%s" : "%s", label.c_str());
    ImGui::NextColumn();
    ImGui::Spacing();

    ImGui::PushItemWidth(-1);
    ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isInconsistent);
    bool result = ImGui::DragScalarCustomN(
        "", ImGuiDataType_Float, v, 3, v_speed, &v_min, &v_max, format, flags
    );
    ImGui::PopItemFlag();
    ImGui::PopItemWidth();
    ImGui::Columns(1);
    ImGui::PopID();
    ImGui::PopStyleVar(2);
    return result;
}

bool dragFloat4(
    const std::string &label,
    float v[4],
    float v_speed,
    float v_min,
    float v_max,
    const char *format,
    ImGuiSliderFlags flags,
    bool isInconsistent
) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));
    ImGui::PushID(label.c_str());
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text(isInconsistent ? "*%s" : "%s", label.c_str());
    ImGui::NextColumn();
    ImGui::Spacing();

    ImGui::PushItemWidth(-1);
    ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isInconsistent);
    bool result = ImGui::DragScalarCustomN(
        "", ImGuiDataType_Float, v, 4, v_speed, &v_min, &v_max, format, flags
    );
    ImGui::PopItemFlag();
    ImGui::PopItemWidth();
    ImGui::Columns(1);
    ImGui::PopID();
    ImGui::PopStyleVar(2);
    return result;
}

bool dragInt(
    const std::string &label,
    int *v,
    float v_speed,
    int v_min,
    int v_max,
    const char *format,
    ImGuiSliderFlags flags,
    bool isInconsistent
) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));

    ImGui::PushID(label.c_str());
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text(isInconsistent ? "*%s" : "%s", label.c_str());
    ImGui::NextColumn();
    ImGui::Spacing();

    ImGui::PushItemWidth(-1);
    ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isInconsistent);
    bool result =
        ImGui::DragScalarCustom("", ImGuiDataType_S32, v, v_speed, &v_min, &v_max, format, flags);
    ImGui::PopItemFlag();
    ImGui::PopItemWidth();
    ImGui::Columns(1);
    ImGui::PopID();
    ImGui::PopStyleVar(2);
    return result;
}

bool dragInt2(
    const std::string &label,
    int v[2],
    float v_speed,
    int v_min,
    int v_max,
    const char *format,
    ImGuiSliderFlags flags,
    bool isInconsistent
) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));
    ImGui::PushID(label.c_str());
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text(isInconsistent ? "*%s" : "%s", label.c_str());
    ImGui::NextColumn();
    ImGui::Spacing();

    ImGui::PushItemWidth(-1);
    ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isInconsistent);
    bool result = ImGui::DragScalarCustomN(
        "", ImGuiDataType_S32, v, 2, v_speed, &v_min, &v_max, format, flags
    );
    ImGui::PopItemFlag();
    ImGui::PopItemWidth();
    ImGui::Columns(1);
    ImGui::PopID();
    ImGui::PopStyleVar(2);
    return result;
}

bool dragInt3(
    const std::string &label,
    int v[3],
    float v_speed,
    int v_min,
    int v_max,
    const char *format,
    ImGuiSliderFlags flags,
    bool isInconsistent
) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));

    ImGui::PushID(label.c_str());
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text(isInconsistent ? "*%s" : "%s", label.c_str());
    ImGui::NextColumn();
    ImGui::Spacing();

    ImGui::PushItemWidth(-1);
    ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isInconsistent);
    bool result = ImGui::DragScalarCustomN(
        "", ImGuiDataType_S32, v, 3, v_speed, &v_min, &v_max, format, flags
    );
    ImGui::PopItemFlag();
    ImGui::PopItemWidth();
    ImGui::Columns(1);
    ImGui::PopID();
    ImGui::PopStyleVar(2);
    return result;
}

bool dragInt4(
    const std::string &label,
    int v[4],
    float v_speed,
    int v_min,
    int v_max,
    const char *format,
    ImGuiSliderFlags flags,
    bool isInconsistent
) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));

    ImGui::PushID(label.c_str());
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text(isInconsistent ? "*%s" : "%s", label.c_str());
    ImGui::NextColumn();
    ImGui::Spacing();

    ImGui::PushItemWidth(-1);
    ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isInconsistent);
    bool result = ImGui::DragScalarCustomN(
        "", ImGuiDataType_S32, v, 4, v_speed, &v_min, &v_max, format, flags
    );
    ImGui::PopItemFlag();
    ImGui::PopItemWidth();
    ImGui::Columns(1);
    ImGui::PopID();
    ImGui::PopStyleVar(2);
    return result;
}

bool checkbox(const std::string &label, bool *v, bool isInconsistent) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));

    ImGui::PushID(label.c_str());
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text(isInconsistent ? "*%s" : "%s", label.c_str());
    ImGui::NextColumn();
    ImGui::Spacing();

    ImGui::PushItemWidth(-1);
    ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isInconsistent);
    bool result = ImGui::Checkbox("", v);
    ImGui::PopItemFlag();
    ImGui::PopItemWidth();
    ImGui::Columns(1);
    ImGui::PopID();
    ImGui::PopStyleVar(2);
    return result;
}

int combo(
    const std::string &label, const std::vector<std::string> &list, int current, bool isInconsistent
) {
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));

    ImGui::PushID(label.c_str());
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text(isInconsistent ? "*%s" : "%s", label.c_str());
    ImGui::NextColumn();
    ImGui::Spacing();

    ImGui::PushItemWidth(-1);
    auto currentStr = list[current];
    if (ImGui::BeginCombo("", currentStr.c_str())) {
        for (int i = 0; i < list.size(); i++) {
            bool isSelected = currentStr == list[i];
            if (ImGui::Selectable(list[i].c_str(), isSelected)) {
                currentStr = list[i];
                current = i;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    ImGui::Columns(1);
    ImGui::PopID();
    ImGui::PopStyleVar(3);
    return current;
}

bool drawVec3Control(
    const std::string &label, glm::vec3 &values, float resetValue, uint32_t inconsistentAxes
) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);

    ImGuiIO &io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];
    bool edited = false;

    ImGui::PushID(label.c_str());
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    shiftCursorY(6.0f);
    ImGui::Text(inconsistentAxes ? "*%s" : "%s", label.c_str());
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
        edited = true;
        values.x = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputItemWidth);
    ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, inconsistentAxes & (uint32_t)VectorAxes::X);
    edited = ImGui::DragScalarCustom(
                 "##X", ImGuiDataType_Float, &values.x, 0.1f, nullptr, nullptr, "%.2f"
             ) ||
             edited;
    ImGui::PopItemFlag();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize)) {
        edited = true;
        values.y = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputItemWidth);
    ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, inconsistentAxes & (uint32_t)VectorAxes::Y);
    edited = ImGui::DragScalarCustom(
                 "##Y", ImGuiDataType_Float, &values.y, 0.1f, nullptr, nullptr, "%.2f"
             ) ||
             edited;
    ImGui::PopItemFlag();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize)) {
        edited = true;
        values.z = resetValue;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputItemWidth);
    ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, inconsistentAxes & (uint32_t)VectorAxes::Z);
    edited = ImGui::DragScalarCustom(
                 "##Z", ImGuiDataType_Float, &values.z, 0.1f, nullptr, nullptr, "%.2f"
             ) ||
             edited;
    ImGui::PopItemFlag();

    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
    ImGui::PopStyleVar();
    return edited;
}

bool propertyColor(const char *label, Color &value, bool isInconsistent) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, coefficientRounding);
    shiftCursorY(6.0f);
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text(isInconsistent ? "*%s" : "%s", label);
    ImGui::NextColumn();
    ImGui::PushItemWidth(-1);
    bool modified = ImGui::ColorEdit4("", &value.r);
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    ImGui::PopStyleVar(2);
    ImGui::Columns(1);
    return modified;
}

bool propertyTexture(
    const char *label, UUID &textureId, Foundation::Shared<Asset> asset, bool isInconsistent
) {
    bool changed = false;
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    shiftCursorY(6.0f);
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text(isInconsistent ? "*%s" : "%s", label);
    ImGui::NextColumn();
    if (textureId) {
        if (asset) {
            auto texture = Foundation::SharedCast<Texture>(asset);
            float height = 55;
            float aspect = texture->getSize().width / texture->getSize().height;
            aspect = Foundation::min(aspect, 4.f);
            float width = height * aspect;
            ImGui::Image((ImTextureID)(intptr_t)texture->getMirenHandle().id, {width, height});
        } else {
            ImGui::Button("Texture Asset", {100, 55});
        }
    } else {
        ImGui::Button("No image", {100, 55});
    }
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
        if (ImGui::GetDragDropPayload() == nullptr) {
            DragDropItem item;
            item.type = DragDropItemType::TEXTURE;
            PND_STATIC_ASSERT(sizeof(textureId) <= sizeof(DragDropItem::data));
            memcpy(item.data, &textureId, sizeof(textureId));
            item.count = 1;
            ImGui::SetDragDropPayload(PANDA_DRAGDROP_NAME, &item, sizeof(DragDropItem));
        }
        ImGui::Text("Texture");
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(PANDA_DRAGDROP_NAME)) {
            PND_ASSERT(payload->DataSize == sizeof(DragDropItem), "WRONG DRAGDROP ITEM SIZE");
            DragDropItem &item = *(DragDropItem *)payload->Data;
            if (item.type == DragDropItemType::TEXTURE) {
                memcpy(&textureId, item.data, sizeof(textureId));
                changed = true;
            }
        }
        ImGui::EndDragDropTarget();
    }
    if (textureId) {
        ImGui::SameLine();
        if (ImGui::Button(getString(ICON_TRASH_O).c_str())) {
            changed = true;
            textureId = 0;
        }
    }
    ImGui::NextColumn();
    ImGui::PopStyleVar();
    ImGui::Columns(1);
    return changed;
}

bool propertyEntity(const char *label, UUID *value) {
    bool changed = false;
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, coefficientRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));

    ImGui::PushID(label);
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, firstColumnWidth);
    ImGui::Text("%s", label);
    ImGui::NextColumn();
    ImGui::Spacing();

    ImGui::PushItemWidth(-1);
    World *currentWorld = GameContext::s_currentWorld;
    if (value && *value && currentWorld) {
        Entity entity = currentWorld->getById(*value);
        ImGui::Text("%s", entity.getName().c_str());
    } else {
        ImGui::Text("%s", "nil");
    }
    ImGui::PopItemWidth();
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
        if (ImGui::GetDragDropPayload() == nullptr) {
            DragDropItem item;
            item.type = DragDropItemType::ENTITY;
            UUID assetId = *value;
            PND_STATIC_ASSERT(sizeof(assetId) <= sizeof(DragDropItem::data));
            memcpy(item.data, &assetId, sizeof(assetId));
            item.count = 1;
            ImGui::SetDragDropPayload(PANDA_DRAGDROP_NAME, &item, sizeof(DragDropItem));
        }
        ImGui::Text("Entity: %d", *(uint32_t *)(value));
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload(PANDA_DRAGDROP_NAME)) {
            PND_ASSERT(payload->DataSize == sizeof(DragDropItem), "WRONG DRAGDROP ITEM SIZE");
            DragDropItem &item = *(DragDropItem *)payload->Data;
            if (item.type == DragDropItemType::ENTITY) {
                memcpy(value, item.data, sizeof(UUID));
                changed = true;
            }
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::PopID();
    if (value && *value) {
        ImGui::SameLine();
        shiftCursorX(15);
        if (ImGui::Button(getString(ICON_TRASH_O).c_str())) {
            changed = true;
            *value = 0;
        }
    }
    ImGui::Columns(1);
    ImGui::PopStyleVar(2);
    return changed;
}

bool drawScriptFieldValue(ScriptField &field) {
    bool changed = false;
    ImGui::PushID(field.fieldId);
    switch (field.type) {
        case ScriptFieldType::INTEGER: {
            changed |= dragInt(field.name.c_str(), (int *)field.value.data);
            break;
        }
        case ScriptFieldType::FLOAT: {
            changed |= dragFloat(field.name.c_str(), (float *)field.value.data, 0.2f);
            break;
        }
        case ScriptFieldType::ENTITY: {
            changed |= propertyEntity(field.name.c_str(), (UUID *)field.value.data);
            break;
        }
        case ScriptFieldType::TEXTURE: {
            // Load texture if it needs.
            AssetHandler *assetHandler = GameContext::s_assetHandler;
            UUID textureId = *(UUID *)field.value.data;
            if (textureId && !field.asset && assetHandler) {
                field.asset = assetHandler->load(textureId);
            }
            if (propertyTexture(
                    field.name.c_str(), *(UUID *)field.value.data, field.asset, false
                )) {
                field.resetCache();
                changed = true;
            }
            break;
        }
        default: {
            ImGui::Text("Unsupported type field: %s", field.name.c_str());
            break;
        }
    }
    ImGui::PopID();
    return changed;
}

} // namespace Panda