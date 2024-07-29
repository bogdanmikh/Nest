#pragma once

#include <cstdint>
#include <filesystem>

#include <Nest.hpp>
#include "Figure.hpp"

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

class Cube : public Figure {
public:
    ~Cube();
    void init(const CreateInfo &createInfo) override;
    void draw(double deltaTime) override;
    void rotateX(float degrees);
    void rotateY(float degrees);
    void rotateZ(float degrees);
private:
    CreateInfo m_createInfo;
    CubeMap m_cubeMap;
    Texture m_texture;
    Shader m_shaderCube;
    Mesh m_mesh;
    glm::mat4 m_model;
};