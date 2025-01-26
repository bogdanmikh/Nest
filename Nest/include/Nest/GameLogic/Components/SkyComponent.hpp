#pragma once

#include "Nest/Application/Application.hpp"
#include "Nest/Assets/AssetLoader.hpp"

#include <Bird/Bird.hpp>
#include <filesystem>
#include <glm/glm.hpp>

namespace Nest {

struct SkyVertex {
    glm::vec3 pos;

    SkyVertex(float x, float y, float z)
        : pos(x, y, z) {}
};

struct SkyCreateInfo {
    std::optional<std::array<Path, 6>> pathsSkyTextures;
    std::optional<Path> pathHdrTexture;
    Path pathToVertexShader;
    Path pathToFragmentShader;
    SkyCreateInfo(
        std::array<Path, 6> pathSkyTextures, Path pathToVertexShader, Path pathToFragmentShader
    )
        : pathsSkyTextures(pathSkyTextures)
        , pathToVertexShader(pathToVertexShader)
        , pathToFragmentShader(pathToFragmentShader) {}

    SkyCreateInfo(Path pathToHdrTexture, Path pathToVertexShader, Path pathToFragmentShader)
        : pathHdrTexture(pathToHdrTexture)
        , pathToVertexShader(pathToVertexShader)
        , pathToFragmentShader(pathToFragmentShader) {}
};

class SkyComponent final {
public:
    SkyComponent(SkyComponent &other) = delete;

    SkyComponent(const SkyCreateInfo &skyCreateInfo)
        : m_sceneViewId(0) {
        using namespace Bird;
        SkyVertex vertices[24] = {
            // Front
            SkyVertex(-1.0f, -1.0f, 1.0f), // 0
            SkyVertex(1.0f, -1.0f, 1.0f),  // 1
            SkyVertex(1.0f, 1.0f, 1.0f),   // 2
            SkyVertex(-1.0f, 1.0f, 1.0f),  // 3
            // Back
            SkyVertex(-1.0f, -1.0f, -1.0f), // 4
            SkyVertex(-1.0f, 1.0f, -1.0f),  // 5
            SkyVertex(1.0f, 1.0f, -1.0f),   // 6
            SkyVertex(1.0f, -1.0f, -1.0f),  // 7
            // Top
            SkyVertex(-1.0f, 1.0f, -1.0f), // 8
            SkyVertex(-1.0f, 1.0f, 1.0f),  // 11
            SkyVertex(1.0f, 1.0f, 1.0f),   // 10
            SkyVertex(1.0f, 1.0f, -1.0f),  // 9
            // Bottom
            SkyVertex(-1.0f, -1.0f, -1.0f), // 12
            SkyVertex(1.0f, -1.0f, -1.0f),  // 13
            SkyVertex(1.0f, -1.0f, 1.0f),   // 14
            SkyVertex(-1.0f, -1.0f, 1.0f),  // 15
            // Left
            SkyVertex(-1.0f, -1.0f, -1.0f), // 16
            SkyVertex(-1.0f, -1.0f, 1.0f),  // 17
            SkyVertex(-1.0f, 1.0f, 1.0f),   // 18
            SkyVertex(-1.0f, 1.0f, -1.0f),  // 19
            // Right
            SkyVertex(1.0f, -1.0f, -1.0f), // 20
            SkyVertex(1.0f, 1.0f, -1.0f),  // 23
            SkyVertex(1.0f, 1.0f, 1.0f),   // 22
            SkyVertex(1.0f, -1.0f, 1.0f)   // 21
        };

        // clang-format off
        uint32_t indices[36] = {
            0, 1, 2, 2, 3, 0,       // Front
            4, 5, 6, 6, 7, 4,       // Back
            8, 9, 10, 10, 11, 8,    // Top
            12, 13, 14, 14, 15, 12, // Bottom
            16, 17, 18, 18, 19, 16, // Left
            20, 21, 22, 22, 23, 20  // Right
        };
        // clang-format on

        Bird::VertexBufferLayoutData layoutData;
        layoutData.pushVec3();
        Bird::VertexLayoutHandle layoutHandle = Bird::createVertexLayout(layoutData);
        Foundation::Memory verticesMemory =
            Foundation::Memory::copying(vertices, sizeof(SkyVertex) * 24);
        Foundation::Memory indicesMemory =
            Foundation::Memory::copying(indices, sizeof(uint32_t) * 36);
        m_vertexBuffer =
            Bird::createVertexBuffer(verticesMemory, 24 * sizeof(SkyVertex), layoutHandle);
        m_indexBuffer =
            Bird::createIndexBuffer(indicesMemory, Bird::BufferElementType::UnsignedInt, 36);
        if (!skyCreateInfo.pathHdrTexture.has_value()) {
            Nest::TextureAsset m_skyTextureAsset = AssetLoader::loadCubeMapTexture(
                {skyCreateInfo.pathsSkyTextures.value()[0].string(),
                 skyCreateInfo.pathsSkyTextures.value()[1].string(),
                 skyCreateInfo.pathsSkyTextures.value()[2].string(),
                 skyCreateInfo.pathsSkyTextures.value()[3].string(),
                 skyCreateInfo.pathsSkyTextures.value()[4].string(),
                 skyCreateInfo.pathsSkyTextures.value()[5].string()}
            );
            Bird::TextureCreate m_skyTextureConfig = m_skyTextureAsset.getBirdTextureCreate();
            m_skyTextureConfig.m_minFiltering = NEAREST;
            m_skyTextureConfig.m_magFiltering = LINEAR;
            m_skyTexture = Bird::createTexture(m_skyTextureConfig);
        } else {
            auto path = skyCreateInfo.pathHdrTexture.value();
            Nest::TextureAsset m_skyTextureAsset = AssetLoader::loadTexture(path);
            Bird::TextureCreate m_skyTextureConfig = m_skyTextureAsset.getBirdTextureCreate();
            m_skyTextureConfig.m_minFiltering = NEAREST;
            m_skyTextureConfig.m_magFiltering = LINEAR;
            m_skyTexture = Bird::createTexture(m_skyTextureConfig);
        }
        ProgramAsset programAsset = AssetLoader::loadProgram(
            skyCreateInfo.pathToVertexShader.string(), skyCreateInfo.pathToFragmentShader.string()
        );
        m_shader = Bird::createProgram(programAsset.getBirdProgramCreate());
        m_model = glm::mat4(1.f);
    }

    ~SkyComponent() {
        Bird::deleteVertexBuffer(m_vertexBuffer);
        Bird::deleteIndexBuffer(m_indexBuffer);
        Bird::deleteProgram(m_shader);
        Bird::deleteTexture(m_skyTexture);
    }

    Bird::TextureHandle getSkyTexture() {
        return m_skyTexture;
    }

    void update(glm::mat4 &viewProjection) {
        m_viewProjection = viewProjection;
        Bird::setShader(m_shader);
        static int samplerCube = 0;
        Bird::setTexture(m_skyTexture, samplerCube);
        Bird::setUniform(m_shader, "skyTexture", &samplerCube, Bird::UniformType::Sampler);
        Bird::setUniform(m_shader, "model", &m_model[0][0], Bird::UniformType::Mat4);
        Bird::setUniform(m_shader, "projViewMtx", &m_viewProjection[0][0], Bird::UniformType::Mat4);
        Bird::setVertexBuffer(m_vertexBuffer);
        Bird::setIndexBuffer(m_indexBuffer, 0, 36);
        Bird::setState(0);
        Bird::submit(m_sceneViewId);
    }

    void setViewId(Bird::ViewId viewId) {
        m_sceneViewId = viewId;
    }

private:
    glm::mat4 m_model;
    glm::mat4 m_viewProjection;
    Bird::ViewId m_sceneViewId;

    Bird::TextureHandle m_skyTexture;
    Bird::ProgramHandle m_shader;
    Bird::IndexBufferHandle m_indexBuffer;
    Bird::VertexBufferHandle m_vertexBuffer;
};

} // namespace Nest