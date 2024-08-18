#pragma once

#include "SphereRenderer.hpp"
#include "CubeRenderer.hpp"
#include "CameraMove.hpp"

#include <Nest.hpp>

class NestPBR final : public Nest::Layer {
public:
    ~NestPBR() override;
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onDetach() override;
    void addEntity(Nest::Entity *entity) override;

private:
//    SkyComponent m_skyComponent;
    CubeRenderer m_cubeRenderer;
    std::vector<SphereRenderer> m_spheres;
    CameraMove m_cameraMove;
};
