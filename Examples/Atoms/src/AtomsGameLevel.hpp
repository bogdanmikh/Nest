#pragma once

#include "StartMenu.hpp"
#include <Nest.hpp>

class AtomsGameLevel : public Nest::Layer {
public:
    ~AtomsGameLevel() override;
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onDetach() override;
    void addEntity(Nest::Entity *entity) override;
private:
    std::vector<Nest::Entity *> m_entities;
    StartMenu m_startMenu;
};

