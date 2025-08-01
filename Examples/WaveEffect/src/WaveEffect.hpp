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

enum class Direction {
    LEFT = 0, RIGHT = 1, COUNT = 2
};
class WaveEffect final : public Nest::Entity {
public:
    WaveEffect();
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

    float maxSpeed = 0.001;
    glm::vec4 background;
    glm::vec2 mousePos;
    glm::vec2 resolution;
    float time;
    Nest::Vec3 colors[6];
    Nest::Vec3 rotation[3];
    Direction direction[3];
    float audio[3];
    float react[3];
    float interaction;
};
