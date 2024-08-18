#pragma once

#include <Nest.hpp>
#include "Figure.hpp"

class CubeRenderer final : public Figure {
public:
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;
private:
    void onAttach() override;
    Bird::TextureHandle m_texture;
    Bird::VertexBufferHandle m_vertexBuffer;
    Bird::IndexBufferHandle m_indexBuffer;
    Bird::ProgramHandle m_shader;
};
