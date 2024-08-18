#pragma once

#include <Nest.hpp>
#include "Figure.hpp"

class SphereRenderer final : public Figure {
public:
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;

private:
    void onAttach() override;
    Bird::VertexBufferHandle m_vertexBuffer;
    Bird::IndexBufferHandle m_indexBuffer;
    size_t m_countIndices;

    /// features
    float metallic;
};
