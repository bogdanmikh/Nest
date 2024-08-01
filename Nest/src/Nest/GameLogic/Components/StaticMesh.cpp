//
// Created by Admin on 12.02.2022.
//

#include "Nest/GameLogic/Components/StaticMesh.hpp"

namespace Nest {

StaticMesh::~StaticMesh() {
    if (m_vertexBufferHandle.isValid()) {
        NestRen::deleteVertexBuffer(m_vertexBufferHandle);
    }
    if (m_indexBufferHandle.isValid()) {
        NestRen::deleteIndexBuffer(m_indexBufferHandle);
    }
}

void StaticMesh::create(
    const Nest::MeshData &data, NestRen::TextureHandle texture, NestRen::ProgramHandle shader
) {
    m_shaderHandle = shader;
    m_textureHandle = texture;
    m_indicesCount = data.indicesCount;
    m_bufferLayoutHandle = data.layoutHandle;
    m_vertexBufferHandle =
        NestRen::createVertexBuffer(data.vertexBuffer, data.vertexBufferSize, m_bufferLayoutHandle);
    m_indexBufferHandle = NestRen::createIndexBuffer(
        data.indices, NestRen::BufferElementType::UnsignedInt, data.indicesCount
    );
}

} // namespace Nest
