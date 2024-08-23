//
// Created by Bogdan on 23.08.2024.
//

#include "AtomsGameLevel.hpp"

AtomsGameLevel::~AtomsGameLevel() {}

void AtomsGameLevel::onAttach() {
    m_player.onAttach();
    m_cameraMove.onAttach();
}

void AtomsGameLevel::onUpdate(double deltaTime) {
    m_player.onUpdate(deltaTime);
    m_cameraMove.onUpdate(deltaTime);
}

void AtomsGameLevel::onDetach() {
    m_player.onDetach();
    m_cameraMove.onDetach();
}

void AtomsGameLevel::addEntity(Nest::Entity *entity) {}
