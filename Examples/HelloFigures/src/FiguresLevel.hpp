//
// Created by Bogdan
//

#pragma once

#include <Nest.hpp>

class SphereRenderer;
class CameraMove;
class CubeRenderer;

class FigureLevel final : public Nest::Layer {
public:
    ~FigureLevel() override;
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onDetach() override;
    void addEntity(Nest::Entity *entity) override;

private:
    std::vector<Nest::Entity *> m_entities;
    SphereRenderer *m_sphereRenderer;
    CubeRenderer *m_cubeRenderer;
    CameraMove *m_cameraMove;
};