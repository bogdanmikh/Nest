//
// Created by Bogdan
//

#pragma once

#include "Nest/Renderer/MeshData.hpp"
#include "Nest/Renderer/TextureBinding.hpp"

#include <Bird/Bird.hpp>

namespace Nest {

class DynamicMesh {
public:
    ~DynamicMesh();
    DynamicMesh();
    DynamicMesh(DynamicMesh &&source);
    DynamicMesh(DynamicMesh &source);

    void
    create(const MeshData &data, std::vector<TextureBinding> bindings, Bird::ProgramHandle shader);
    void update(const MeshData &data);

private:
    std::vector<TextureBinding> m_bindings;
    Bird::ProgramHandle m_shaderHandle;
    Bird::VertexLayoutHandle m_bufferLayoutHandle;
    Bird::IndexBufferHandle m_indexBufferHandle;
    Bird::VertexBufferHandle m_vertexBufferHandle;
    glm::mat4 m_model;
    uint32_t m_indicesCount;

    friend class Renderer3D;
    friend class Model3D;
};

} // namespace Nest
