#pragma once

#include <array>
#include <Bird/Base.hpp>
#include <filesystem>
#include <glm/glm.hpp>
#include "Bird/Bird.hpp"
#include "Nest/Base/Base.hpp"
#include "Nest/Assets/AssetLoader.hpp"
#include "Nest/GameLogic/Entity.hpp"
#include "Nest/GameLogic/Components/TransformComponent.hpp"

struct CreateInfo {
    bool useTexture;
    bool useCubeMap;
    Bird::TextureHandle skyComponentHandle;
    glm::vec3 position;
    Nest::Path pathToTexture;
    Nest::Path pathToVertexShader;
    Nest::Path pathToFragmentShader;
    std::string nameTexture;
    std::string nameSkyTexture;
    CreateInfo()
        : useTexture(false)
        , useCubeMap(false)
        , position(0, 0, 0) {}
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
    void onAttachFigure(const CreateInfo &createInfo) {
        m_createInfo = createInfo;
        m_transformComponent.setPosition(m_createInfo.position);

        /// create shader
        Nest::ProgramAsset programAsset = Nest::AssetLoader::loadProgram(
            m_createInfo.pathToVertexShader.string(), m_createInfo.pathToFragmentShader.string()
        );
        m_shader = Bird::createProgram(programAsset.getBirdProgramCreate());

        /// create texture
        if (m_createInfo.useTexture) {
            Nest::TextureAsset textureAsset =
                Nest::AssetLoader::loadTexture(m_createInfo.pathToTexture.string());

            Bird::TextureCreate textureCreate = textureAsset.getBirdTextureCreate();
            textureCreate.m_numMips = 4;
            textureCreate.m_minFiltering = Bird::NEAREST_MIPMAP_LINEAR;
            textureCreate.m_magFiltering = Bird::NEAREST;
            m_texture = Bird::createTexture(textureCreate);
        }

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
    Bird::TextureHandle m_texture;
    Bird::ProgramHandle m_shader;
};