//
// Created by Bogdan on 29.09.2024.
//

#include "PostprocessingEffect.hpp"

PostprocessingEffect::PostprocessingEffect()
    : m_viewId(0) {}

void PostprocessingEffect::onAttach() {
    using namespace Bird;

    Nest::ProgramAsset programAsset = Nest::AssetLoader::loadProgram(m_vertexPath, m_fragmentPath);
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

void PostprocessingEffect::onUpdate(double deltaTime) {
    time = Nest::Application::get()->getWindow()->getTime();
    mousePos = {Nest::Input::getMousePositionX(), Nest::Input::getMousePositionY()};
    auto size = Nest::Application::get()->getWindow()->getSize();
    size *= Nest::Application::get()->getWindow()->getDpi();
    resolution = size;

    Bird::setShader(m_shader);
    Bird::setUniform(m_shader, "iTimeVec4", &time,
                     Bird::UniformType::Vec4); /// float
    Bird::setUniform(m_shader, "iResolutionVec4", &resolution,
                     Bird::UniformType::Vec4); /// vec2
    Bird::setUniform(m_shader, "iMouseVec4", &mousePos,
                     Bird::UniformType::Vec4); /// vec2
    if (m_textureHandle.isValid()) {
        Bird::setTexture(m_textureHandle, slot);
        Bird::setUniform(m_shader, "iChannel0", &slot, Bird::UniformType::Sampler);
    }

    Bird::setShader(m_shader);
    Bird::setIndexBuffer(m_indexBuffer, 0, 6);
    Bird::setVertexBuffer(m_vertexBuffer);
    Bird::submit(m_viewId);
}

void PostprocessingEffect::onImGuiRender() {}

void PostprocessingEffect::onDetach() {
    deleteVertexBuffer(m_vertexBuffer);
    deleteIndexBuffer(m_indexBuffer);
    deleteProgram(m_shader);
}

void PostprocessingEffect::setPathToShaders(
    const std::string &vertexPath, const std::string &fragmentPath
) {
    m_vertexPath = vertexPath;
    m_fragmentPath = fragmentPath;
}

void PostprocessingEffect::setFBTexture(Bird::TextureHandle textureHandle) {
    m_textureHandle = textureHandle;
}

void PostprocessingEffect::setViewId(Bird::ViewId viewId) {
    m_viewId = viewId;
}