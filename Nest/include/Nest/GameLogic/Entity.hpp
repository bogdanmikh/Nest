//
// Created by Admin on 09.02.2022
//

#pragma once

namespace Nest {

class Entity {
public:
    Entity() = default;
    virtual ~Entity() = default;

    virtual void onAttach() = 0;
    virtual void onDetach() = 0;
    virtual void onUpdate(double deltaTime) = 0;
    virtual void onImGuiRender() = 0;
};

} // namespace Nest