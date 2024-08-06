//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Nest/Renderer/MeshData.hpp"

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
        , m_textureHandle(BIRD_INVALID_HANDLE)
        , m_shaderHandle(BIRD_INVALID_HANDLE) {}
    ~StaticMesh();

    void
    create(const MeshData &data, Bird::TextureHandle texture, Bird::ProgramHandle shader);

private:
    Bird::TextureHandle m_textureHandle;
    Bird::ProgramHandle m_shaderHandle;
    Bird::VertexLayoutHandle m_bufferLayoutHandle;
    Bird::IndexBufferHandle m_indexBufferHandle;
    Bird::VertexBufferHandle m_vertexBufferHandle;
    glm::mat4 m_model;
    uint32_t m_indicesCount;

    friend class Renderer3D;
};

} // namespace Nest
