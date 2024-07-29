#pragma once

#include <Nest.hpp>
#include "Figure.hpp"
#include <filesystem>

struct SphereVertex {
    // position
    glm::vec3 Position;
    // texCoords
    glm::vec2 TexCoords;
    // normal
    glm::vec3 Normal;

    SphereVertex()
        : Position(0, 0, 0)
        , TexCoords(0, 0)
        , Normal(0, 0, 0) {};

    SphereVertex(glm::vec3 aPosition, glm::vec2 aTexCoord, glm::vec3 aNormal)
        : Position(aPosition)
        , TexCoords(aTexCoord)
        , Normal(aNormal) {};

    SphereVertex(
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

struct Settings {
    float metallic = 0.0;
    int countSpheres = 0;
};

class Sphere : public Figure {
public:
    ~Sphere();
    void init(const CreateInfo &createInfo) override;
    void draw(double deltaTime) override;
    void translate(glm::vec3 v);
    void translate(float x, float y, float z);
    static void drawSettings();
private:
    static Settings m_settings;
    CreateInfo m_createInfo;
    CubeMap m_cubeMap;
    Texture m_texture;
    Shader m_sphereShader;
    Mesh m_mesh;
    glm::mat4 m_model;
    glm::vec3 m_position;
};