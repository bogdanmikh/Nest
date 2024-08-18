#pragma once

#include <filesystem>
#include <array>
#include <glm/glm.hpp>

struct CreateInfo {
    bool useTexture;
    bool useCubeMap;
    glm::vec3 position;
    std::filesystem::path pathToTexture;
    std::array<std::string, 6> skyTextureAsset;
    std::filesystem::path pathToVertexShader;
    std::filesystem::path pathToFragmentShader;
    std::string nameTexture;
    std::string nameCubeMap;
    CreateInfo()
    : useTexture(false)
    , useCubeMap(false)
    , position(0, 0, 0)
    , nameCubeMap("") {}
};


struct VertexFigure {
    // position
    glm::vec3 Position;
    // texCoords
    glm::vec2 TexCoords;
    // normal
    glm::vec3 Normal;

    VertexFigure()
        : Position(0, 0, 0)
        , TexCoords(0, 0)
        , Normal(0, 0, 0) {};

    VertexFigure(glm::vec3 aPosition, glm::vec2 aTexCoord, glm::vec3 aNormal)
        : Position(aPosition)
        , TexCoords(aTexCoord)
        , Normal(aNormal) {};

    VertexFigure(
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


struct Figure : public Nest::Entity {
public:
    void onAttachFigure(const CreateInfo& createInfo) {
        m_createInfo = createInfo;
        onAttach();
    }
    Nest::TransformComponent &getTransform() {
        return m_transformComponent;
    }

    virtual void onUpdate(double deltaTime) override = 0;
    virtual void onImGuiRender() override = 0;
    virtual void onDetach() override = 0;
    virtual ~Figure() override {}
protected:
    virtual void onAttach() override = 0;
    CreateInfo m_createInfo;
    Nest::TransformComponent m_transformComponent;
};