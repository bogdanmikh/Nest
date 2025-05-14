//
// Created by Bogdan
//

#pragma once

#include <Nest.hpp>

class TriangleRenderer;

class FigureLevel final : public Nest::Layer {
public:
    ~FigureLevel() override;
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onDetach() override;
    void addEntity(Nest::Entity *entity);

private:
    std::vector<Nest::Entity *> m_entities;
    TriangleRenderer *m_triangleRenderer;
};