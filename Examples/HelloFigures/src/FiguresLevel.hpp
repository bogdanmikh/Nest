//
// Created by Admin on 19/03/2022.
//

#pragma once

#include <Nest.hpp>

class FigureLevel final : public Nest::Layer {
public:
    ~FigureLevel() override;
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onDetach() override;
    void addEntity(Nest::Entity *entity) override;
private:
    std::vector<Nest::Entity*> m_entities;
};