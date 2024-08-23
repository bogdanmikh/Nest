//
// Created by Bogdan on 23.08.2024.
//

#include "AtomsGameLevel.hpp"

AtomsGameLevel::~AtomsGameLevel() {}

void AtomsGameLevel::onAttach() {}

void AtomsGameLevel::onUpdate(double deltaTime) {
    m_startMenu.showStartMenu();
}

void AtomsGameLevel::onDetach() {}

void AtomsGameLevel::addEntity(Nest::Entity *entity) {}
