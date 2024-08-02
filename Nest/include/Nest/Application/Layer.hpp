#pragma once

#include <string>
#include "Nest/GameLogic/Entity.hpp"
#include "Foundation/Allocator.hpp"

namespace Nest {

class Layer {
public:
    Layer() = default;
    ~Layer() {
        onDetach();
    }

    void onAttach() {
        for (auto &entity : m_entities) {
            entity->onAttach();
        }
    }

    void onUpdate(double deltaTime) {
        for (auto &entity : m_entities) {
            entity->onImGuiRender();
            entity->onUpdate(deltaTime);
        }
    }

    void onDetach() {
        for (auto &entity : m_entities) {
            entity->onDetach();
        }
    }
    void addEntity(Entity *entity) {
        m_entities.emplace_back(entity);
    }
private:
    std::vector<Entity*> m_entities;
};

} // namespace Nest