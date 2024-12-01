#pragma once

#include <Nest.hpp>

class ProfilerTest final : public Nest::Layer {
public:
    ~ProfilerTest() override;
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onDetach() override;
    void addEntity(Nest::Entity *entity) override;
};
