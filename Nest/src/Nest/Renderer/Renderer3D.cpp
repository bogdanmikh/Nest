#include "Nest/Renderer/Renderer3D.hpp"

#include <glm/glm.hpp>

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
    //    Bird::setShader(mesh->m_shaderHandle);
    updateModel(transform, mesh->m_model);
    Bird::setUniform(mesh->m_shaderHandle, "model", &mesh->m_model[0][0], Bird::UniformType::Mat4);
    Bird::setUniform(
        mesh->m_shaderHandle, "projViewMtx", (void *)&m_viewProj, Bird::UniformType::Mat4
    );
    for (int i = 0; i < mesh->m_textureBinding.size(); i++) {
        Bird::setTexture(mesh->m_textureBinding[i].texture, i);
        Bird::setUniform(
            mesh->m_shaderHandle,
            mesh->m_textureBinding[i].name.c_str(),
            &i,
            Bird::UniformType::Sampler
        );
    }

    NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
    Bird::setVertexBuffer(mesh->m_vertexBufferHandle);
    NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
    Bird::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
    Bird::submit(0);
    m_drawData.stats.drawCalls += 1;
}

void Renderer3D::submitToFB(TransformComponent *transform, StaticMesh *mesh, Bird::ViewId viewId) {
    NEST_ASSERT(mesh->m_shaderHandle.isValid(), "Invalid shader for mesh");
    //    Bird::setShader(mesh->m_shaderHandle);
    updateModel(transform, mesh->m_model);
    Bird::setUniform(mesh->m_shaderHandle, "model", &mesh->m_model[0][0], Bird::UniformType::Mat4);
    Bird::setUniform(
        mesh->m_shaderHandle, "projViewMtx", (void *)&m_viewProj, Bird::UniformType::Mat4
    );
    static int slot = 0;
    for (int i = 0; i < mesh->m_textureBinding.size(); i++) {
        Bird::setTexture(mesh->m_textureBinding[i].texture, slot);
        Bird::setUniform(
            mesh->m_shaderHandle,
            mesh->m_textureBinding[i].name.c_str(),
            &slot,
            Bird::UniformType::Sampler
        );
    }

    NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
    Bird::setVertexBuffer(mesh->m_vertexBufferHandle);
    NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
    Bird::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
    Bird::submit(viewId);
    m_drawData.stats.drawCalls += 1;
}

void Renderer3D::submit(TransformComponent *transform, DynamicMesh *mesh) {
    NEST_ASSERT(mesh->m_shaderHandle.isValid(), "Invalid shader for mesh");
    Bird::setShader(mesh->m_shaderHandle);
    updateModel(transform, mesh->m_model);
    Bird::setUniform(mesh->m_shaderHandle, "model", &mesh->m_model[0][0], Bird::UniformType::Mat4);
    Bird::setUniform(
        mesh->m_shaderHandle, "projViewMtx", (void *)&m_viewProj, Bird::UniformType::Mat4
    );
    static int slot = 0;
    for (int i = 0; i < mesh->m_bindings.size(); i++) {
        Bird::setTexture(mesh->m_bindings[i].texture, slot);
        Bird::setUniform(
            mesh->m_shaderHandle,
            mesh->m_bindings[i].name.c_str(),
            &slot,
            Bird::UniformType::Sampler
        );
    }
    NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
    Bird::setVertexBuffer(mesh->m_vertexBufferHandle);
    NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
    Bird::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
    Bird::submit(0);
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