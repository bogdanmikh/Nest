#include "WaveEffectLevel.hpp"
#include "WaveEffect.hpp"

#include <Nest.hpp>

WaveEffectLevel::~WaveEffectLevel() {}

void WaveEffectLevel::onAttach() {
    m_waveEffect = F_NEW(Foundation::getAllocator(), WaveEffect);

    addEntity(m_waveEffect);
    for (auto &entity : m_entities) {
        entity->onAttach();
    }
}

void WaveEffectLevel::onUpdate(double deltaTime) {
    for (auto &entity : m_entities) {
        entity->onUpdate(deltaTime);
        entity->onImGuiRender();
    }
}

void WaveEffectLevel::onDetach() {
    for (auto &entity : m_entities) {
        entity->onDetach();
        F_DELETE(Foundation::getAllocator(), entity);
    }
}

void WaveEffectLevel::addEntity(Nest::Entity *entity) {
    m_entities.emplace_back(entity);
}