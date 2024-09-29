//
// Created by Bogdan on 29.09.2024.
//

#include "DrunkEffect.hpp"

void DrunkEffect::onAttach() {
    using namespace Bird;

    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vstDrunk.glsl", "Shaders/fstDrunk.glsl");
    m_shader = createProgram(programAsset.getBirdProgramCreate());

    // clang-format off
    float vertices[8] = {
        // x   y
        -1.0, -1.0,
        1.0, -1.0,
        -1.0, 1.0,
        1.0, 1.0,
    };
    // clang-format on

    Foundation::Memory verticesMemory = Foundation::Memory::copying(vertices, sizeof(float) * 8);

    uint32_t indices[6] = {0, 1, 2, 1, 3, 2};
    Foundation::Memory indicesMemory = Foundation::Memory::copying(indices, sizeof(uint32_t) * 6);

    VertexBufferLayoutData layoutData;
    layoutData.pushVec2();
    VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);
    m_vertexBuffer = createVertexBuffer(verticesMemory, 8 * sizeof(float), vertexLayout);
    m_indexBuffer = createIndexBuffer(indicesMemory, BufferElementType::UnsignedInt, 6);
}

void DrunkEffect::onUpdate(double deltaTime) {
    time = Nest::Application::get()->getWindow()->getTime();
    mousePos = Nest::Events::getCursorPos();
    resolution = Nest::Application::get()->getWindow()->getSize();

    Bird::setShader(m_shader);
    Bird::setUniform(m_shader, "iTimeVec4", &time, Bird::UniformType::Vec4);             /// float
    Bird::setUniform(m_shader, "iResolutionVec4", &resolution, Bird::UniformType::Vec4); /// vec2
    Bird::setUniform(m_shader, "iMouseVec4", &mousePos, Bird::UniformType::Vec4);        /// vec2

    Bird::setShader(m_shader);
    Bird::setIndexBuffer(m_indexBuffer, 0, 6);
    Bird::setVertexBuffer(m_vertexBuffer);
    Bird::submit(0);
}

void DrunkEffect::onImGuiRender() {}

void DrunkEffect::onDetach() {
    deleteVertexBuffer(m_vertexBuffer);
    deleteIndexBuffer(m_indexBuffer);
    deleteProgram(m_shader);
}
