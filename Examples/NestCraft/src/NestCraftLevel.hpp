#pragma once

#include "CoreGame/GameObject.hpp"
#include "CoreGame/Menu.hpp"
#include <Nest.hpp>

class NestCraftLevel final : public Nest::Layer {
public:
    ~NestCraftLevel() override {}
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onDetach() override;
    void addEntity(Nest::Entity *entity) override;

private:
    Menu m_menu;
    Nest::Viewport m_viewport;
    std::vector<Nest::Entity *> m_entities;
};
