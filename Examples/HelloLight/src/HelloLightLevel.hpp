#pragma once

#include <Nest.hpp>
#include "Objects/LightEnvironment.hpp"

class HelloLightLevel final : public Nest::Layer {
public:
    ~HelloLightLevel() override {}
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onDetach() override;
    void addEntity(Nest::Entity *entity);

private:
    std::vector<Nest::Entity *> m_entities;
};