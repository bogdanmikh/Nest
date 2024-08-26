#pragma once

#include <Nest.hpp>

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

class SphereRenderer final : public Nest::Entity {
public:
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;
    Nest::TransformComponent &getTransform();

private:
    Bird::TextureHandle m_texture;
    Bird::VertexBufferHandle m_vertexBuffer;
    Bird::IndexBufferHandle m_indexBuffer;
    Bird::ProgramHandle m_shader;
    size_t m_countIndices;

    Nest::TransformComponent m_transformComponent;

    // uniforms
    Nest::WorldCamera *m_camera;
    glm::mat4 projViewMtx;
    glm::mat4 model;
    glm::vec3 cameraPos;
    glm::vec2 mousePos;
    glm::vec2 resolution;
    double time;
};
