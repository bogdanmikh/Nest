//
// Created by Bogdan
//

#pragma once

#include <Nest.hpp>

class WaveEffect;

class WaveEffectLevel final : public Nest::Layer {
public:
    ~WaveEffectLevel() override;
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onDetach() override;
    void addEntity(Nest::Entity *entity);

private:
    std::vector<Nest::Entity *> m_entities;
    WaveEffect *m_waveEffect;
};