#include "Nest/Renderer/Model3D.hpp"

namespace Nest {
TransformComponent &Model3D::getTransform() {
    return m_transformComponent;
}

void Model3D::create(Bird::ProgramHandle shader, Path pathToModel) {}

void Model3D::draw() {
    for (int i = 0; i < m_meshes.size(); ++i) {
        auto *mesh = &m_meshes[i];
        NEST_ASSERT(mesh->m_shaderHandle.isValid(), "Invalid shader for mesh");
        //    Bird::setShader(mesh->m_shaderHandle);
        mesh->m_model = getTransform().getTransform();
        Bird::setUniform(
            mesh->m_shaderHandle, "model", &mesh->m_model[0][0], Bird::UniformType::Mat4
        );
        Bird::setUniform(
            mesh->m_shaderHandle, "projViewMtx", (void *)&m_viewProj, Bird::UniformType::Mat4
        );
        for (int j = 0; j < mesh->m_bindings.size(); j++) {
            Bird::setTexture(mesh->m_bindings[j].texture, j);
            Bird::setUniform(
                mesh->m_shaderHandle,
                mesh->m_bindings[j].name.c_str(),
                &j,
                Bird::UniformType::Sampler
            );
        }

        NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid vertex buffer for mesh");
        Bird::setVertexBuffer(mesh->m_vertexBufferHandle);
        NEST_ASSERT(mesh->m_vertexBufferHandle.isValid(), "Invalid index buffer for mesh");
        Bird::setIndexBuffer(mesh->m_indexBufferHandle, 0, mesh->m_indicesCount);
        Bird::submit(0);
    }
}
} // namespace Nest