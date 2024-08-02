//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Nest/Renderer/MeshData.hpp"

#include <NestRen/NestRen.hpp>

namespace Nest {

struct MeshData;

class StaticMesh {
public:
    StaticMesh()
        : m_model(1.f)
        , m_bufferLayoutHandle(NESTREN_INVALID_HANDLE)
        , m_indexBufferHandle(NESTREN_INVALID_HANDLE)
        , m_vertexBufferHandle(NESTREN_INVALID_HANDLE)
        , m_indicesCount(0)
        , m_textureHandle(NESTREN_INVALID_HANDLE)
        , m_shaderHandle(NESTREN_INVALID_HANDLE) {}
    ~StaticMesh();

    void create(const MeshData &data, NestRen::TextureHandle texture, NestRen::ProgramHandle shader);

private:
    NestRen::TextureHandle m_textureHandle;
    NestRen::ProgramHandle m_shaderHandle;
    NestRen::VertexLayoutHandle m_bufferLayoutHandle;
    NestRen::IndexBufferHandle m_indexBufferHandle;
    NestRen::VertexBufferHandle m_vertexBufferHandle;
    glm::mat4 m_model;
    uint32_t m_indicesCount;

    friend class Renderer3D;
};

} // namespace Nest
