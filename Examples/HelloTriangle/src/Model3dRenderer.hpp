#pragma once

#include <Nest.hpp>

class Model3dRenderer final : public Nest::Entity {
public:
    Model3dRenderer();
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;
    Nest::TransformComponent &getTransform();

private:
    Bird::ProgramHandle m_shader;
    Nest::TransformComponent m_transformComponent;
    Bird::IndexBufferHandle m_indexBuffer;
    Bird::VertexBufferHandle m_vertexBuffer;

    Nest::Model3D m_model;
};
