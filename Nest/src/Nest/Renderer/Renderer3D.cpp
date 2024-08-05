#include "Nest/Renderer/Renderer3D.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Nest {

Renderer3D::Renderer3D()
    : m_viewProj(1.f)
    , m_drawData() {}

Renderer3D::Renderer3D(Nest::Renderer3D &&other)
    : m_viewProj(other.m_viewProj)
    , m_drawData(other.m_drawData) {}

void updateModel(Nest::TransformComponent *transform, glm::mat4 &model) {
    model = transform->getTransform();
}

void Renderer3D::begin() {
    m_drawData.stats.drawCalls = 0;
}

void Renderer3D::submit(TransformComponent *transform, StaticMesh *mesh) {
    NEST_ASSERT(mesh->m_shaderHandle.isValid(), "Invalid shader for mesh");
    NestRen::setShader(mesh->m_shaderHandle);
    updateModel(transform, mesh->m_model);
    NestRen::setUniform(
        mesh->m_shaderHandle, "model", &mesh->m_model[0][0], NestRen::UniformType::Mat4
    );
    NestRen::setUniform(
        mesh->m_shaderHandle, "projViewMtx", (void *)&m_viewProj, NestRen::UniformType::Mat4
    );
    NestRen::setTexture(mesh->m_textureHandle, 0);
    NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
    NestRen::setVertexBuffer(mesh->m_vertexBufferHandle);
    NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
    NestRen::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
    NestRen::submit(0);
    m_drawData.stats.drawCalls += 1;
}

void Renderer3D::submit(TransformComponent *transform, DynamicMesh *mesh) {
    NEST_ASSERT(mesh->m_shaderHandle.isValid(), "Invalid shader for mesh");
    NestRen::setShader(mesh->m_shaderHandle);
    updateModel(transform, mesh->m_model);
    NestRen::setUniform(
        mesh->m_shaderHandle, "model", &mesh->m_model[0][0], NestRen::UniformType::Mat4
    );
    NestRen::setUniform(
        mesh->m_shaderHandle, "projViewMtx", (void *)&m_viewProj, NestRen::UniformType::Mat4
    );
    for (int i = 0; i < mesh->m_bindings.size(); i++) {
        NestRen::setTexture(mesh->m_bindings[i].texture, i);
        NestRen::setUniform(
            mesh->m_shaderHandle, mesh->m_bindings[i].name.c_str(), &i, NestRen::UniformType::Sampler
        );
    }
    NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
    NestRen::setVertexBuffer(mesh->m_vertexBufferHandle);
    NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
    NestRen::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
    NestRen::submit(0);
    m_drawData.stats.drawCalls += 1;
}

void Renderer3D::end() {}

Renderer3D::Statistics Renderer3D::getStats() {
    return m_drawData.stats;
}

void Renderer3D::setViewProj(glm::mat4 viewProj) {
    m_viewProj = viewProj;
}

Renderer3D &Renderer3D::operator=(Renderer3D &&other) {
    m_viewProj = other.m_viewProj;
    return *this;
}

} // namespace Nest