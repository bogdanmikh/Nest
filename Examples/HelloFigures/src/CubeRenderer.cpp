#include "CubeRenderer.hpp"

CubeRenderer::CubeRenderer() {}

void CubeRenderer::onAttach() {
    m_camera = Nest::Application::get()->getWorldCamera();

    using namespace Bird;
    Nest::Path vertexShaderPath, fragmentShaderPath;

    vertexShaderPath = "Shaders/vstSphere.glsl";
    fragmentShaderPath = "Shaders/fstSphere.glsl";
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram(vertexShaderPath, fragmentShaderPath);
    m_shader = createProgram(programAsset.getBirdProgramCreate());

    Nest::TextureAsset textureAsset = Nest::AssetLoader::loadTexture("Textures/Rust.jpg");

    TextureCreate textureCreate = textureAsset.getBirdTextureCreate();
    textureCreate.m_numMips = 4;
    textureCreate.m_minFiltering = Bird::NEAREST_MIPMAP_LINEAR;
    textureCreate.m_magFiltering = Bird::NEAREST;
    m_texture = createTexture(textureCreate);

    // clang-format off
    VertexCube vertices[24] = {
        // Front face
        VertexCube(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),  // 0
        VertexCube(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f), // 1
        VertexCube(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),// 2
        VertexCube(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f), // 3

        // Back face
        VertexCube(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f), // 4
        VertexCube(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f),// 5
        VertexCube(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f), // 6
        VertexCube(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f),  // 7

        // Left face
        VertexCube(-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f), // 8
        VertexCube(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f),// 9
        VertexCube(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),// 10
        VertexCube(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f), // 11

        // Right face
        VertexCube(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f), // 12
        VertexCube(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f),  // 13
        VertexCube(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f), // 14
        VertexCube(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),// 15

        // Top face
        VertexCube(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f), // 16
        VertexCube(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),  // 17
        VertexCube(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f), // 18
        VertexCube(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),// 19

        // Bottom face
        VertexCube(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f), // 20
        VertexCube(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f),  // 21
        VertexCube(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f),   // 22
        VertexCube(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f)   // 23
    };
    Foundation::Memory verticesMemory = Foundation::Memory::copying(vertices, sizeof(VertexCube) * 24);

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

    VertexBufferLayoutData layoutData;
    layoutData.pushVec3();
    layoutData.pushVec2();
    layoutData.pushVec3();
    VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);
    m_vertexBuffer = createVertexBuffer(verticesMemory, 24 * sizeof(VertexCube), vertexLayout);
    m_indexBuffer = createIndexBuffer(indicesMemory, BufferElementType::UnsignedInt, 36);
}

void CubeRenderer::onUpdate(double deltaTime) {
    time = Nest::Application::get()->getWindow()->getTime();
    mousePos = {Nest::Input::getMousePositionX(), Nest::Input::getMousePositionX()};
    resolution = Nest::Application::get()->getWindow()->getSize();

    projViewMtx = m_camera->getProjectionMatrix() * m_camera->getViewMatrix();

    cameraPos = m_camera->getPosition();

    model = m_transformComponent.getTransform();

    Bird::setShader(m_shader);
    Bird::setUniform(m_shader, "iTimeVec4", &time,
                     Bird::UniformType::Vec4); /// float
    Bird::setUniform(m_shader, "iResolutionVec4", &resolution,
                     Bird::UniformType::Vec4); /// vec2
    Bird::setUniform(m_shader, "iMouseVec4", &mousePos,
                     Bird::UniformType::Vec4); /// vec2
    Bird::setUniform(m_shader, "iCameraPosVec4", &cameraPos,
                     Bird::UniformType::Vec4);                            /// vec4
    Bird::setUniform(m_shader, "model", &model, Bird::UniformType::Mat4); /// mat4
    Bird::setUniform(m_shader, "projViewMtx", &projViewMtx,
                     Bird::UniformType::Mat4); /// mat4
    static int slot = 0;
    Bird::setTexture(m_texture, slot);
    Bird::setUniform(m_shader, "iTexture", &slot, Bird::UniformType::Sampler);
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

Nest::TransformComponent &CubeRenderer::getTransform() {
    return m_transformComponent;
}