//
// Created by Bogdan on 19.03.2025.
//

#pragma once

#include <Nest.hpp>

struct InfoObject {
    Nest::Vec3 position = {0, 0, 0};
    Nest::Vec3 scale = {1, 1, 1};
    Nest::Vec3 degrees = {0, 0, 0};

    Nest::Path pathToModel = "";
    Nest::CreateInfoModel3D createInfoModel3D;
};

class ManagerObjects {
public:
    void add(const InfoObject &infoObject);
    std::vector<Nest::Model3D> &getModels();

private:
    std::vector<Nest::Model3D> m_models;
};
