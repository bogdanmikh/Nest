#pragma once

#include <Nest.hpp>

struct VertexTriangle {
    // position
    glm::vec3 Position;
    VertexTriangle(glm::vec3 pos)
        : Position(pos) {}
    VertexTriangle(float x, float y, float z)
        : Position(x, y, z) {}
};

class TriangleRenderer final : public Nest::Entity {
public:
    TriangleRenderer();
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;
    Nest::TransformComponent &getTransform();

private:
    Bird::ProgramHandle m_shader;
    Nest::TransformComponent m_transformComponent;
    Bird::IndexBufferHandle m_indexBuffer;
    Bird::VertexBufferHandle m_vertexBuffer;
};
