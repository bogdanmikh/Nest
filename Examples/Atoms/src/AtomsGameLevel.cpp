//
// Created by Bogdan on 23.08.2024.
//

#include "AtomsGameLevel.hpp"

AtomsGameLevel::~AtomsGameLevel() {}

void AtomsGameLevel::onAttach() {
    auto worldCamera = Nest::Application::get()->getWorldCamera();
    worldCamera->setPosition(0, 0, 40);
    m_player.onAttach();
}

void AtomsGameLevel::onUpdate(double deltaTime) {
    m_player.onUpdate(deltaTime);
    m_player.onMove(deltaTime);
    m_player.setColor({Nest::getRandomFloat(0., 1.), 1., 0.});
}

void AtomsGameLevel::onDetach() {
    m_player.onDetach();
}