#include "CubeRenderer.hpp"

void CubeRenderer::onAttach() {
    m_camera = Nest::Application::get()->getWorldCamera();

    // clang-format off
    VertexFigure vertices[24] = {
        // Front face
        VertexFigure(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),  // 0
        VertexFigure(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f), // 1
        VertexFigure(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),// 2
        VertexFigure(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f), // 3

        // Back face
        VertexFigure(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f), // 4
        VertexFigure(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f),// 5
        VertexFigure(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f), // 6
        VertexFigure(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f),  // 7

        // Left face
        VertexFigure(-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f), // 8
        VertexFigure(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f),// 9
        VertexFigure(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),// 10
        VertexFigure(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f), // 11

        // Right face
        VertexFigure(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f), // 12
        VertexFigure(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f),  // 13
        VertexFigure(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f), // 14
        VertexFigure(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),// 15

        // Top face
        VertexFigure(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f), // 16
        VertexFigure(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),  // 17
        VertexFigure(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f), // 18
        VertexFigure(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),// 19

        // Bottom face
        VertexFigure(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f), // 20
        VertexFigure(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f),  // 21
        VertexFigure(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f),   // 22
        VertexFigure(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f)   // 23
    };
    Foundation::Memory verticesMemory = Foundation::Memory::copying(vertices, sizeof(VertexFigure) * 24);

    uint32_t indices[36] {
       0, 1, 2, 2, 3, 0,       // Front
       4, 5, 6, 6, 7, 4,       // Back
       8, 9, 10, 10, 11, 8,    // Top
       12, 13, 14, 14, 15, 12, // Bottom
       16, 17, 18, 18, 19, 16, // Left
       20, 21, 22, 22, 23, 20  // Right
    };
    // clang-format on
    Foundation::Memory indicesMemory = Foundation::Memory::copying(indices, sizeof(uint32_t) * 36);

    Bird::VertexBufferLayoutData layoutData;
    layoutData.pushVec3();
    layoutData.pushVec2();
    layoutData.pushVec3();
    Bird::VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);
    m_vertexBuffer = createVertexBuffer(verticesMemory, 24 * sizeof(VertexFigure), vertexLayout);
    m_indexBuffer = createIndexBuffer(indicesMemory, Bird::BufferElementType::UnsignedInt, 36);
}

void CubeRenderer::onUpdate(double deltaTime) {
    time = Nest::Application::get()->getWindow()->getTime();
    mousePos = {Nest::Input::getMousePositionX(), Nest::Input::getMousePositionY()};
    resolution = Nest::Application::get()->getWindow()->getSize();

    projViewMtx = m_camera->getProjectionMatrix() * m_camera->getViewMatrix();

    cameraPos = m_camera->getPosition();

    model = m_transformComponent.getTransform();

    static auto color = glm::vec3(0.);

    static float metallic = 0.5;

    Bird::setShader(m_shader);
    Bird::setUniform(m_shader, "iTimeVec4", &time,
                     Bird::UniformType::Vec4); /// float
    Bird::setUniform(m_shader, "iResolutionVec4", &resolution,
                     Bird::UniformType::Vec4); /// vec2
    Bird::setUniform(m_shader, "iMouseVec4", &mousePos,
                     Bird::UniformType::Vec4); /// vec2
    Bird::setUniform(m_shader, "iCameraPosVec4", &cameraPos,
                     Bird::UniformType::Vec4); /// vec4
    Bird::setUniform(m_shader, "iColorVec4", &color,
                     Bird::UniformType::Vec4);                            /// vec3
    Bird::setUniform(m_shader, "model", &model, Bird::UniformType::Mat4); /// mat4
    Bird::setUniform(m_shader, "projViewMtx", &projViewMtx,
                     Bird::UniformType::Mat4); /// mat4
    Bird::setUniform(m_shader, "iMetallicVec4", &metallic,
                     Bird::UniformType::Vec4); /// float

    /// texture using
    if (m_createInfo.useTexture) {
        static int slot = 0;
        Bird::setTexture(m_texture, slot);
        Bird::setUniform(
            m_shader, m_createInfo.nameTexture.c_str(), &slot, Bird::UniformType::Sampler
        );
    }
    /// cubemap using
    if (m_createInfo.useCubeMap) {
        static int slot = 1;
        Bird::setTexture(m_createInfo.skyComponentHandle, slot);
        Bird::setUniform(
            m_shader, m_createInfo.nameSkyTexture.c_str(), &slot, Bird::UniformType::Sampler
        );
    }

    Bird::setIndexBuffer(m_indexBuffer, 0, 36);
    Bird::setVertexBuffer(m_vertexBuffer);
    Bird::submit(0);
}

void CubeRenderer::onImGuiRender() {}

void CubeRenderer::onDetach() {
    deleteVertexBuffer(m_vertexBuffer);
    deleteIndexBuffer(m_indexBuffer);
    deleteProgram(m_shader);
    deleteTexture(m_texture);
}