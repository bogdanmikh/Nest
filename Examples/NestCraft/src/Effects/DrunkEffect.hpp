//
// Created by Bogdan on 29.09.2024.
//

#pragma once

#include <Nest.hpp>

class DrunkEffect : public Nest::Entity {
public:
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;
    void onDetach() override;

private:
    Bird::VertexBufferHandle m_vertexBuffer;
    Bird::IndexBufferHandle m_indexBuffer;
    Bird::ProgramHandle m_shader;

    // uniforms
    glm::vec2 mousePos;
    glm::vec2 resolution;
    double time;
};
