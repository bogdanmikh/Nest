#include "Panda/Renderer/Renderer3D.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Panda {

Renderer3D::Renderer3D()
    : m_viewId(0)
    , m_viewProj(1.f)
    , m_drawData() {}

Renderer3D::Renderer3D(Panda::Renderer3D &&other)
    : m_viewId(other.m_viewId)
    , m_viewProj(other.m_viewProj)
    , m_drawData(other.m_drawData) {}

void updateModel(Panda::TransformComponent *transform, glm::mat4 &model) {
    model = transform->getTransform();
}

void Renderer3D::begin() {
    m_drawData.stats.drawCalls = 0;
}

void Renderer3D::submit(TransformComponent *transform, StaticMesh *mesh) {
    PND_ASSERT(mesh->m_shaderHandle.isValid(), "Invalid shader for mesh");
    Miren::setShader(mesh->m_shaderHandle);
    updateModel(transform, mesh->m_model);
    Miren::setUniform(
        mesh->m_shaderHandle, "model", &mesh->m_model[0][0], Miren::UniformDataType::Mat4
    );
    Miren::setUniform(
        mesh->m_shaderHandle, "projViewMtx", (void *)&m_viewProj, Miren::UniformDataType::Mat4
    );
    Miren::setTexture(mesh->m_textureHandle, 0);
    PND_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
    Miren::setVertexBuffer(mesh->m_vertexBufferHandle);
    PND_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
    Miren::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
    Miren::submit(m_viewId);
    m_drawData.stats.drawCalls += 1;
}

void Renderer3D::submit(TransformComponent *transform, DynamicMesh *mesh) {
    PND_ASSERT(mesh->m_shaderHandle.isValid(), "Invalid shader for mesh");
    Miren::setShader(mesh->m_shaderHandle);
    updateModel(transform, mesh->m_model);
    Miren::setUniform(
        mesh->m_shaderHandle, "model", &mesh->m_model[0][0], Miren::UniformDataType::Mat4
    );
    Miren::setUniform(
        mesh->m_shaderHandle, "projViewMtx", (void *)&m_viewProj, Miren::UniformDataType::Mat4
    );
    for(int i = 0; i < mesh->m_textures.size(); i++) {
        Miren::setTexture(mesh->m_textures[i], 0);
    }
    PND_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
    Miren::setVertexBuffer(mesh->m_vertexBufferHandle);
    PND_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
    Miren::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
    Miren::submit(m_viewId);
    m_drawData.stats.drawCalls += 1;
}

void Renderer3D::end() {}

Renderer3D::Statistics Renderer3D::getStats() {
    return m_drawData.stats;
}

void Renderer3D::setViewId(Miren::ViewId id) {
    m_viewId = id;
}

void Renderer3D::setViewProj(glm::mat4 viewProj) {
    m_viewProj = viewProj;
}

Renderer3D &Renderer3D::operator=(Renderer3D &&other) {
    m_viewId = other.m_viewId;
    m_viewProj = other.m_viewProj;
    return *this;
}

} // namespace Panda