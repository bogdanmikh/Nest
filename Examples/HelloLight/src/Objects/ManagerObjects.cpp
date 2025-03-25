//
// Created by Bogdan on 19.03.2025.
//

#include "ManagerObjects.hpp"

void ManagerObjects::add(const InfoObject &infoObject) {
    m_models.emplace_back();

    Nest::Model3D &model = m_models.back();
    model.create(ProgramHandle(), infoObject.pathToModel, infoObject.createInfoModel3D);
    auto &transform = model.getTransform();
    transform.setPosition(Nest::Vec3(infoObject.position));
    transform.setScale(Nest::Vec3(infoObject.scale));
    transform.setRotationEuler(Nest::Vec3(infoObject.degrees));
}

std::vector<Nest::Model3D> &ManagerObjects::getModels() {
    return m_models;
}
