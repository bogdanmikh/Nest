#pragma once

#include "Nest/Assets/AssetLoader.hpp"
#include "Nest/Application/Application.hpp"

#include <glm/glm.hpp>
#include <NestRen/NestRen.hpp>

namespace Nest {

struct SkyVertex {
    glm::vec3 pos;

    SkyVertex(float x, float y, float z)
        : pos(x, y, z) {}
};

class SkyComponent final {
public:
    SkyComponent(SkyComponent &other) = delete;

    SkyComponent()
        : m_sceneViewId(0) {
        using namespace NestRen;
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

        NestRen::VertexBufferLayoutData layoutData;
        layoutData.pushVec3();
        NestRen::VertexLayoutHandle layoutHandle = NestRen::createVertexLayout(layoutData);
        Foundation::Memory verticesMemory =
            Foundation::Memory::copying(vertices, sizeof(SkyVertex) * 24);
        Foundation::Memory indicesMemory =
            Foundation::Memory::copying(indices, sizeof(uint32_t) * 36);
        m_vertexBuffer =
            NestRen::createVertexBuffer(verticesMemory, 24 * sizeof(SkyVertex), layoutHandle);
        m_indexBuffer =
            NestRen::createIndexBuffer(indicesMemory, NestRen::BufferElementType::UnsignedInt, 36);

        Nest::TextureAsset m_skyTextureAsset = AssetLoader::loadCubeMapTexture({
            "default-textures/skybox/px.png",
            "default-textures/skybox/nx.png",
            "default-textures/skybox/py.png",
            "default-textures/skybox/ny.png",
            "default-textures/skybox/pz.png",
            "default-textures/skybox/nz.png",
        });
        NestRen::TextureCreate m_skyTextureConfig = m_skyTextureAsset.getNestRenTextureCreate();
        m_skyTextureConfig.m_minFiltering = NEAREST;
        m_skyTextureConfig.m_magFiltering = LINEAR;
        m_skyTexture = NestRen::createTexture(m_skyTextureConfig);

        ProgramAsset programAsset = AssetLoader::loadProgram(
            "default-shaders/sky/sky_vertex.glsl", "default-shaders/sky/sky_fragment.glsl"
        );
        m_shader = NestRen::createProgram(programAsset.getNestRenProgramCreate());
        m_model = glm::mat4(1.f);
    }

    ~SkyComponent() {
        NestRen::deleteVertexBuffer(m_vertexBuffer);
        NestRen::deleteIndexBuffer(m_indexBuffer);
        NestRen::deleteProgram(m_shader);
        NestRen::deleteTexture(m_skyTexture);
    }

    NestRen::TextureHandle getSkyTexture() {
        return m_skyTexture;
    }

    void update(glm::mat4 &viewProjection) {
        m_viewProjection = viewProjection;
        NestRen::setShader(m_shader);
        static int samplerCube = 0;
        NestRen::setTexture(m_skyTexture, samplerCube);
        NestRen::setUniform(m_shader, "skyTexture", &samplerCube, NestRen::UniformType::Int);
        NestRen::setUniform(m_shader, "model", &m_model[0][0], NestRen::UniformType::Mat4);
        NestRen::setUniform(
            m_shader, "projViewMtx", &m_viewProjection[0][0], NestRen::UniformType::Mat4
        );
        NestRen::setVertexBuffer(m_vertexBuffer);
        NestRen::setIndexBuffer(m_indexBuffer, 0, 36);
        NestRen::setState(0);
        NestRen::submit(m_sceneViewId);
    }

    void setViewId(NestRen::ViewId viewId) {
        m_sceneViewId = viewId;
    }

private:
    glm::mat4 m_model;
    glm::mat4 m_viewProjection;
    NestRen::ViewId m_sceneViewId;

    NestRen::TextureHandle m_skyTexture;
    NestRen::ProgramHandle m_shader;
    NestRen::IndexBufferHandle m_indexBuffer;
    NestRen::VertexBufferHandle m_vertexBuffer;
};

} // namespace Nest