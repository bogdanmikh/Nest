#pragma once

#include "SkyComponent.hpp"
#include "SkyComponent.hpp"
#include "Cube.hpp"
#include "Sphere.hpp"
#include "CameraMove.hpp"

#include <Nest.hpp>

class Profiler final : public Layer {
public:
    void start() override;
    void detach() override;
    void update(double deltaTime) override;

private:
    SkyComponent m_skyComponent;
    Cube m_cube;
    std::vector<Sphere> m_spheres;
    CameraMove m_cameraMove;
};
