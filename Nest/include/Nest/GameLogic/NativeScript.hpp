//
// Created by Admin on 09.02.2022.
//

#pragma once

#include <Foundation/Foundation.hpp>

#include "Entity.hpp"

namespace Nest {

class NativeScript {
public:
    NativeScript()
        : m_entity(nullptr, -1, nullptr)
        , m_isActive(true) {}

    virtual ~NativeScript() = default;

    void setEntity(Entity entity) {
        m_entity = entity;
    }

    Entity getEntity() {
        return m_entity;
    }

    virtual void initialize() = 0;

    virtual void update(double deltaTime) = 0;

    virtual void onImGuiRender() {};

    bool m_isActive;

private:
    Entity m_entity;
};

} // namespace Panda