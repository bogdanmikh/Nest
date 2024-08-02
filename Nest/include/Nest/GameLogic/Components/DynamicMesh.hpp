//
// Created by Admin on 12.02.2022.
//

#pragma once

#include "Nest/Renderer/MeshData.hpp"

#include <NestRen/NestRen.hpp>

namespace Nest {

class DynamicMesh {
public:
    ~DynamicMesh();
    DynamicMesh();
    DynamicMesh(DynamicMesh &&source);
    DynamicMesh(DynamicMesh &source);

    void create(const MeshData &data, std::vector<NestRen::TextureHandle> textures, NestRen::ProgramHandle shader);
    void update(const MeshData &data);

private:
    std::vector<NestRen::TextureHandle> m_textures;
    NestRen::ProgramHandle m_shaderHandle;
    NestRen::VertexLayoutHandle m_bufferLayoutHandle;
    NestRen::IndexBufferHandle m_indexBufferHandle;
    NestRen::VertexBufferHandle m_vertexBufferHandle;
    glm::mat4 m_model;
    uint32_t m_indicesCount;

    friend class Renderer3D;
};

} // namespace Nest
