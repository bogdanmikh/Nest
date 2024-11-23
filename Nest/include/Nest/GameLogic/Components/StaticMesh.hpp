//
// Created by Bogdan
//

#pragma once

#include "Nest/Renderer/MeshData.hpp"
#include "Nest/Renderer/TextureBinding.hpp"

#include <Bird/Bird.hpp>

namespace Nest {

struct MeshData;

class StaticMesh {
public:
    StaticMesh()
        : m_model(1.f)
        , m_bufferLayoutHandle(BIRD_INVALID_HANDLE)
        , m_indexBufferHandle(BIRD_INVALID_HANDLE)
        , m_vertexBufferHandle(BIRD_INVALID_HANDLE)
        , m_indicesCount(0)
        , m_shaderHandle(BIRD_INVALID_HANDLE) {}
    ~StaticMesh();

    void create(const MeshData &data, TextureBinding textureBinding, Bird::ProgramHandle shader);
    void create(
        const MeshData &data, std::vector<TextureBinding> textureBinding, Bird::ProgramHandle shader
    );

    Bird::ProgramHandle getShaderHandle() {
        return m_shaderHandle;
    }

    void deleteTextures();

private:
    std::vector<TextureBinding> m_textureBinding;
    Bird::ProgramHandle m_shaderHandle;
    Bird::VertexLayoutHandle m_bufferLayoutHandle;
    Bird::IndexBufferHandle m_indexBufferHandle;
    Bird::VertexBufferHandle m_vertexBufferHandle;
    glm::mat4 m_model;
    uint32_t m_indicesCount;

    friend class Renderer3D;
    friend class Model3D;
    friend class NaniteModel3D;
};

} // namespace Nest
