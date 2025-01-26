#pragma once

#include "Figure.hpp"
#include <Nest.hpp>

class SphereRenderer final : public Figure {
public:
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;

private:
    void onAttach() override;
    Bird::VertexBufferHandle m_vertexBuffer;
    Bird::IndexBufferHandle m_indexBuffer;
    size_t m_countIndices;

    /// uniforms
    float metallic;
    Nest::WorldCamera *m_camera;
    glm::mat4 projViewMtx;
    glm::mat4 model;
    glm::vec3 cameraPos;
    glm::vec2 mousePos;
    glm::vec2 resolution;
    double time;
};
