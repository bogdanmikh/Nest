#pragma once

#include <string>
#include "Nest/GameLogic/Entity.hpp"
#include "Foundation/Allocator.hpp"

namespace Nest {

class Layer {
public:
    virtual ~Layer() = default;
    virtual void onAttach() = 0;
    virtual void onUpdate(double deltaTime) = 0;
    virtual void onDetach() = 0;
    virtual void addEntity(Entity *entity) = 0;

private:
    std::vector<Entity *> m_entities;
};

} // namespace Nest