//
// Created by Admin on 12.02.2022.
//

#include "Nest/GameLogic/Components/StaticMesh.hpp"

namespace Nest {

StaticMesh::~StaticMesh() {
    if (m_vertexBufferHandle.isValid()) {
        Bird::deleteVertexBuffer(m_vertexBufferHandle);
    }
    if (m_indexBufferHandle.isValid()) {
        Bird::deleteIndexBuffer(m_indexBufferHandle);
    }
}

void StaticMesh::create(
    const Nest::MeshData &data, Bird::TextureHandle texture, Bird::ProgramHandle shader
) {
    m_shaderHandle = shader;
    m_textureHandle = texture;
    m_indicesCount = data.indicesCount;
    m_bufferLayoutHandle = data.layoutHandle;
    m_vertexBufferHandle =
        Bird::createVertexBuffer(data.vertexBuffer, data.vertexBufferSize, m_bufferLayoutHandle);
    m_indexBufferHandle = Bird::createIndexBuffer(
        data.indices, Bird::BufferElementType::UnsignedInt, data.indicesCount
    );
}

} // namespace Nest
