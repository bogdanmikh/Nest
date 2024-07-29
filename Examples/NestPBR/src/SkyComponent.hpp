#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include <Nest.hpp>

struct SkyVertex {
    glm::vec3 pos;

    SkyVertex(float x, float y, float z)
        : pos(x, y, z) {}
};

class SkyComponent {
public:
    ~SkyComponent();
    void init(const std::array<std::string, 6> &skyTextureAsset);
    CubeMap getCubeMap();
    void draw();

private:
    Mesh m_mesh;
    CubeMap m_cubeMap;
    Shader *m_shaderCubeMap;
};