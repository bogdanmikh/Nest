#pragma once

#include <Nest.hpp>

struct VertexCircle {
    // position
    glm::vec3 Position;
    // texCoords
    glm::vec2 TexCoords;
    // normal
    glm::vec3 Normal;

    VertexCircle()
        : Position(0, 0, 0)
        , TexCoords(0, 0)
        , Normal(0, 0, 0) {};

    VertexCircle(glm::vec3 aPosition, glm::vec2 aTexCoord, glm::vec3 aNormal)
        : Position(aPosition)
        , TexCoords(aTexCoord)
        , Normal(aNormal) {};
    VertexCircle(
        float x,
        float y,
        float z,
        float aTexCoordX,
        float aTexCoordY,
        float aNormalX,
        float aNormalY,
        float aNormalZ
    )
        : Position(x, y, z)
        , TexCoords(aTexCoordX, aTexCoordY)
        , Normal(aNormalX, aNormalY, aNormalZ) {};
};

class Player {
public:
    void onAttach();
    void onUpdate(double deltaTime);
    void onImGuiRender();
    void onDetach();
    Nest::TransformComponent &getTransform();
    void setColor(glm::vec3 color);
    void onMove(double deltaTime);

private:
    glm::vec3 m_color;
    constexpr const static float speed = 30.;
    Bird::TextureHandle m_texture;
    Bird::VertexBufferHandle m_vertexBuffer;
    Bird::IndexBufferHandle m_indexBuffer;
    Bird::ProgramHandle m_shader;
    Nest::TransformComponent m_transformComponent;

    size_t m_countIndices;
};
