#pragma once

#include <Nest.hpp>
#include "Objects/LightObject.hpp"

class HelloLightLevel final : public Nest::Layer {
public:
    ~HelloLightLevel() override {}
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onDetach() override;
    void addEntity(Nest::Entity *entity);

private:
    void addLightObject(const InfoLightObject &info);
    std::vector<Nest::Entity *> m_entities;
};