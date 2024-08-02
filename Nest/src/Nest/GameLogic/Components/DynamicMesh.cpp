//
// Created by Admin on 12.02.2022.
//

#include "Nest/GameLogic/Components/DynamicMesh.hpp"

namespace Nest {

DynamicMesh::~DynamicMesh() {
    if (m_vertexBufferHandle.isValid()) {
        NestRen::deleteVertexBuffer(m_vertexBufferHandle);
    }
    if (m_indexBufferHandle.isValid()) {
        NestRen::deleteIndexBuffer(m_indexBufferHandle);
    }
}

DynamicMesh::DynamicMesh(DynamicMesh &&source)
    : m_model(source.m_model)
    , m_bufferLayoutHandle(source.m_bufferLayoutHandle)
    , m_indexBufferHandle(source.m_indexBufferHandle)
    , m_vertexBufferHandle(source.m_vertexBufferHandle)
    , m_indicesCount(source.m_indicesCount)
    , m_textures(source.m_textures)
    , m_shaderHandle(source.m_shaderHandle) {
    source.m_vertexBufferHandle = NESTREN_INVALID_HANDLE;
    source.m_indexBufferHandle = NESTREN_INVALID_HANDLE;
}

DynamicMesh::DynamicMesh()
    : m_model(1.f)
    , m_bufferLayoutHandle(NESTREN_INVALID_HANDLE)
    , m_indexBufferHandle(NESTREN_INVALID_HANDLE)
    , m_vertexBufferHandle(NESTREN_INVALID_HANDLE)
    , m_indicesCount(0)
    , m_textures()
    , m_shaderHandle(NESTREN_INVALID_HANDLE) {}

DynamicMesh::DynamicMesh(DynamicMesh &source)
    : m_model(source.m_model)
    , m_bufferLayoutHandle(source.m_bufferLayoutHandle)
    , m_indexBufferHandle(source.m_indexBufferHandle)
    , m_vertexBufferHandle(source.m_vertexBufferHandle)
    , m_indicesCount(source.m_indicesCount)
    , m_textures(source.m_textures)
    , m_shaderHandle(source.m_shaderHandle) {}

void DynamicMesh::create(
    const Nest::MeshData &data, std::vector<NestRen::TextureHandle> textures, NestRen::ProgramHandle shader
) {
    NEST_ASSERT(shader.isValid(), "Invalid shader for mesh");
    m_shaderHandle = shader;
    m_textures = textures;
    m_indicesCount = data.indicesCount;
    m_bufferLayoutHandle = data.layoutHandle;
    m_vertexBufferHandle = NestRen::createDynamicVertexBuffer(
        data.vertexBuffer, data.vertexBufferSize, m_bufferLayoutHandle
    );
    m_indexBufferHandle = NestRen::createDynamicIndexBuffer(
        data.indices, NestRen::BufferElementType::UnsignedInt, data.indicesCount
    );
}

void DynamicMesh::update(const MeshData &data) {
    NEST_ASSERT(m_shaderHandle.isValid(), "Invalid shader for mesh");
    m_indicesCount = data.indicesCount;
    NestRen::updateDynamicVertexBuffer(
        m_vertexBufferHandle, data.vertexBuffer, data.vertexBufferSize
    );
    NestRen::updateDynamicIndexBuffer(m_indexBufferHandle, data.indices, m_indicesCount);
}

} // namespace Nest
