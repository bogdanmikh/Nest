#pragma once

#include "Foundation/Allocator.hpp"
#include "Nest/GameLogic/Entity.hpp"
#include "Nest/Events/Event.hpp"
#include <string>

namespace Nest {

class Layer {
public:
    Layer(const std::string name = "Layer");
    virtual ~Layer() = default;

    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void onUpdate(double deltaTime) {}
    virtual void onImGuiRender() {}
    virtual void onEvent(Event *event) {}

    const std::string &getName() const {
        return m_debugName;
    }

    virtual void addEntity(Entity *entity) {}

protected:
    std::string m_debugName;
    std::vector<Entity *> m_entities;
};

} // namespace Nest