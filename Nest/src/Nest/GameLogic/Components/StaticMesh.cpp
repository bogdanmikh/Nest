//
// Created by Admin on 12.02.2022.
//

#include "Panda/GameLogic/Components/StaticMesh.hpp"

namespace Panda {

StaticMesh::~StaticMesh() {
    if (m_vertexBufferHandle.isValid()) {
        Miren::deleteVertexBuffer(m_vertexBufferHandle);
    }
    if (m_indexBufferHandle.isValid()) {
        Miren::deleteIndexBuffer(m_indexBufferHandle);
    }
}

void StaticMesh::create(
    const Panda::MeshData &data, Miren::TextureHandle texture, Miren::ProgramHandle shader
) {
    m_shaderHandle = shader;
    m_textureHandle = texture;
    m_indicesCount = data.indicesCount;
    m_bufferLayoutHandle = data.layoutHandle;
    m_vertexBufferHandle =
        Miren::createVertexBuffer(data.vertexBuffer, data.vertexBufferSize, m_bufferLayoutHandle);
    m_indexBufferHandle = Miren::createIndexBuffer(
        data.indices, Miren::BufferElementType::UnsignedInt, data.indicesCount
    );
}

} // namespace Panda
