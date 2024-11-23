//
// Created by Bogdan
//

#include "Nest/GameLogic/Components/StaticMesh.hpp"

#include <utility>

namespace Nest {

StaticMesh::~StaticMesh() {
    if (m_vertexBufferHandle.isValid()) {
        Bird::deleteVertexBuffer(m_vertexBufferHandle);
    }
    if (m_indexBufferHandle.isValid()) {
        Bird::deleteIndexBuffer(m_indexBufferHandle);
    }
    if (m_bufferLayoutHandle.isValid()) {
        deleteVertexLayout(m_bufferLayoutHandle);
    }
}

void StaticMesh::deleteTextures() {
    for (const auto &texture : m_textureBinding) {
        deleteTexture(texture.texture);
    }
}

void StaticMesh::create(
    const Nest::MeshData &data, TextureBinding textureBinding, Bird::ProgramHandle shader
) {
    m_shaderHandle = shader;
    m_textureBinding.emplace_back(std::move(textureBinding));
    m_indicesCount = data.indicesCount;
    m_bufferLayoutHandle = data.layoutHandle;
    m_vertexBufferHandle =
        Bird::createVertexBuffer(data.vertexBuffer, data.vertexBufferSize, m_bufferLayoutHandle);
    m_indexBufferHandle = Bird::createIndexBuffer(
        data.indices, Bird::BufferElementType::UnsignedInt, data.indicesCount
    );
}

void StaticMesh::create(
    const Nest::MeshData &data,
    std::vector<TextureBinding> textureBinding,
    Bird::ProgramHandle shader
) {
    m_shaderHandle = shader;
    m_textureBinding = std::move(textureBinding);
    m_indicesCount = data.indicesCount;
    m_bufferLayoutHandle = data.layoutHandle;
    m_vertexBufferHandle =
        Bird::createVertexBuffer(data.vertexBuffer, data.vertexBufferSize, m_bufferLayoutHandle);
    m_indexBufferHandle = Bird::createIndexBuffer(
        data.indices, Bird::BufferElementType::UnsignedInt, data.indicesCount
    );
}

} // namespace Nest
