#pragma once

#include <Nest.hpp>

struct VertexCube {
    // position
    glm::vec3 Position;
    // texCoords
    glm::vec2 TexCoords;
    // normal
    glm::vec3 Normal;

    VertexCube()
        : Position(0, 0, 0)
        , TexCoords(0, 0)
        , Normal(0, 0, 0) {};

    VertexCube(glm::vec3 aPosition, glm::vec2 aTexCoord, glm::vec3 aNormal)
        : Position(aPosition)
        , TexCoords(aTexCoord)
        , Normal(aNormal) {};

    VertexCube(
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

class CubeRenderer final : public Nest::Entity {
public:
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;
    void rotateX(float degrees);
    void rotateY(float degrees);
    void rotateZ(float degrees);
private:
    NestRen::TextureHandle m_texture;
    NestRen::VertexBufferHandle m_vertexBuffer;
    NestRen::IndexBufferHandle m_indexBuffer;
    NestRen::ProgramHandle m_shader;
    glm::mat4 m_model;
};
