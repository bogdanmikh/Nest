#pragma once

#include <Nest.hpp>

class HelloLightLevel final : public Nest::Layer {
public:
    ~HelloLightLevel() override {}
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onDetach() override;
    void addEntity(Nest::Entity *entity) override;

private:
    std::vector<Nest::Entity *> m_entities;
};