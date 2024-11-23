//
// Created by Bogdan
//

#include "Nest/GameLogic/Components/DynamicMesh.hpp"

namespace Nest {

DynamicMesh::~DynamicMesh() {
    if (m_vertexBufferHandle.isValid()) {
        Bird::deleteVertexBuffer(m_vertexBufferHandle);
    }
    if (m_indexBufferHandle.isValid()) {
        Bird::deleteIndexBuffer(m_indexBufferHandle);
    }
    if (m_bufferLayoutHandle.isValid()) {
        deleteVertexLayout(m_bufferLayoutHandle);
    }
    for (const auto &texture : m_bindings) {
        deleteTexture(texture.texture);
    }
}

DynamicMesh::DynamicMesh(DynamicMesh &&source)
    : m_model(source.m_model)
    , m_bufferLayoutHandle(source.m_bufferLayoutHandle)
    , m_indexBufferHandle(source.m_indexBufferHandle)
    , m_vertexBufferHandle(source.m_vertexBufferHandle)
    , m_indicesCount(source.m_indicesCount)
    , m_bindings(source.m_bindings)
    , m_shaderHandle(source.m_shaderHandle) {
    source.m_vertexBufferHandle = BIRD_INVALID_HANDLE;
    source.m_indexBufferHandle = BIRD_INVALID_HANDLE;
}

DynamicMesh::DynamicMesh()
    : m_model(1.f)
    , m_bufferLayoutHandle(BIRD_INVALID_HANDLE)
    , m_indexBufferHandle(BIRD_INVALID_HANDLE)
    , m_vertexBufferHandle(BIRD_INVALID_HANDLE)
    , m_indicesCount(0)
    , m_bindings()
    , m_shaderHandle(BIRD_INVALID_HANDLE) {}

DynamicMesh::DynamicMesh(DynamicMesh &source)
    : m_model(source.m_model)
    , m_bufferLayoutHandle(source.m_bufferLayoutHandle)
    , m_indexBufferHandle(source.m_indexBufferHandle)
    , m_vertexBufferHandle(source.m_vertexBufferHandle)
    , m_indicesCount(source.m_indicesCount)
    , m_bindings(source.m_bindings)
    , m_shaderHandle(source.m_shaderHandle) {}

void DynamicMesh::create(
    const Nest::MeshData &data, std::vector<TextureBinding> bindings, Bird::ProgramHandle shader
) {
    NEST_ASSERT(shader.isValid(), "Invalid shader for mesh");
    m_shaderHandle = shader;
    m_bindings = bindings;
    m_indicesCount = data.indicesCount;
    m_bufferLayoutHandle = data.layoutHandle;
    m_vertexBufferHandle = Bird::createDynamicVertexBuffer(
        data.vertexBuffer, data.vertexBufferSize, m_bufferLayoutHandle
    );
    m_indexBufferHandle = Bird::createDynamicIndexBuffer(
        data.indices, Bird::BufferElementType::UnsignedInt, data.indicesCount
    );
}

void DynamicMesh::update(const MeshData &data) {
    NEST_ASSERT(m_shaderHandle.isValid(), "Invalid shader for mesh");
    m_indicesCount = data.indicesCount;
    Bird::updateDynamicVertexBuffer(m_vertexBufferHandle, data.vertexBuffer, data.vertexBufferSize);
    Bird::updateDynamicIndexBuffer(m_indexBufferHandle, data.indices, m_indicesCount);
}

} // namespace Nest
