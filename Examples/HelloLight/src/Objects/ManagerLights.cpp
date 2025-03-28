//
// Created by Bogdan on 24.03.2025.
//

#include "ManagerLights.hpp"

void ManagerLights::add(const DirLight &dirLight) {
    m_dirLights.emplace_back(dirLight);
}

void ManagerLights::add(const PointLight &pointLight) {
    m_pointLights.emplace_back(pointLight);
}

void ManagerLights::add(const SpotLight &spotLight) {
    m_spotLights.emplace_back(spotLight);
}

std::vector<DirLight> &ManagerLights::getDirLights() {
    return m_dirLights;
}

std::vector<PointLight> &ManagerLights::getPointLights() {
    return m_pointLights;
}

std::vector<SpotLight> &ManagerLights::getSpotLights() {
    return m_spotLights;
}