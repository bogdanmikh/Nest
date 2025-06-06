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
    ManagerObjects();
    std::vector<Nest::Model3D> &getModels();

    void add(const InfoObject &infoObject);
    void setShader(Bird::ProgramHandle shader);
    void setViewId(Bird::ViewId viewId);
    void draw();

private:
    std::vector<Nest::Model3D> m_models;
};
