//
// Created by Admin on 22.07.2023.
//

#pragma once

#include "Nest/Base/Base.hpp"

#include "Nest/GameLogic/Components/StaticMesh.hpp"
#include "Nest/GameLogic/Components/DynamicMesh.hpp"
#include "WorldCamera.hpp"

#include <Foundation/Foundation.hpp>
#include <Rain/Rain.hpp>
#include <Rain/Codable.hpp>

namespace Nest {

struct IdComponent final : public Rain::Codable {
    id_t id = 0;

    IdComponent() = default;
    IdComponent(const IdComponent &other) = default;
    IdComponent(const id_t &id)
        : id(id) {}

    RAIN_FIELDS_BEGIN(IdComponent)
    RAIN_FIELD(id)
    RAIN_FIELDS_END
};

struct TagComponent final : public Rain::Codable {
    std::string tag;

    TagComponent() = default;
    TagComponent(const TagComponent &other) = default;
    TagComponent(const std::string &tag)
        : tag(tag) {}

    operator std::string &() {
        return tag;
    }
    operator const std::string &() const {
        return tag;
    }

    RAIN_FIELDS_BEGIN(TagComponent)
    RAIN_FIELD(tag)
    RAIN_FIELDS_END
};

struct RelationshipComponent final : public Rain::Codable {
    id_t parentHandle = -1;
    std::vector<id_t> children;

    RelationshipComponent() = default;
    RelationshipComponent(const RelationshipComponent &other) = default;
    RelationshipComponent(id_t parent)
        : parentHandle(parent) {}

    RAIN_FIELDS_BEGIN(RelationshipComponent)
    RAIN_FIELD(parentHandle)
    RAIN_FIELD(children)
    RAIN_FIELDS_END
};

struct SpriteRendererComponent final : public Rain::Codable {
    Color color;

    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent &other) = default;

    RAIN_FIELDS_BEGIN(SpriteRendererComponent)
    RAIN_FIELD(color)
    RAIN_FIELDS_END
};

struct StaticMeshComponent final {
    std::vector<StaticMesh> meshes;
};

struct DynamicMeshComponent final {
    std::vector<DynamicMesh> meshes;

    DynamicMeshComponent() = default;
    DynamicMeshComponent(const DynamicMeshComponent &other) = default;
    DynamicMeshComponent &operator=(DynamicMeshComponent &other) = default;
};

struct CameraComponent final {
    bool isPrimary = true;
    WorldCamera camera;
};

class NativeScript;

struct NativeScriptContainer final {
    Foundation::Shared<NativeScript> instance = nullptr;
    bool initialized = false;
    const char *scriptName;

    template<typename T>
    T &bind() {
        Foundation::Shared<T> script = Foundation::makeShared<T>();
        instance = script;
        scriptName = typeid(T).name();
        return *script;
    }
};

struct NativeScriptListComponent final {
    std::vector<NativeScriptContainer> scripts;

    template<typename T>
    T &add() {
        NativeScriptContainer &container = scripts.emplace_back();
        return container.bind<T>();
    }
};

} // namespace Panda
