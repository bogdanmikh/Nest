#include "CubeRenderer.hpp"

void CubeRenderer::onAttach() {
    using namespace NestRen;

    m_model = glm::mat4(1);
    NestRen::setViewClear(0, 0x3D75C9FF);

    Nest::ProgramAsset programAsset = Nest::AssetLoader::loadProgram(
        "Shaders/vstCube.glsl",
        "Shaders/fstCube.glsl"
    );
    m_shader = createProgram(programAsset.getNestRenProgramCreate());

    Nest::TextureAsset textureAsset = Nest::AssetLoader::loadTexture("Textures/Dubil.png");

    TextureCreate textureCreate = textureAsset.getNestRenTextureCreate();
    textureCreate.m_numMips = 4;
    textureCreate.m_minFiltering = NestRen::NEAREST_MIPMAP_LINEAR;
    textureCreate.m_magFiltering = NestRen::NEAREST;
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
    rotateZ(0.5);
    rotateY(0.5);
    static auto camera = Nest::Application::get()->getWorldCamera();
    static auto time = Nest::Application::get()->getWindow()->getTime();
    time = Nest::Application::get()->getWindow()->getTime();
    static auto mousePos = Nest::Events::getCursorPos();
    mousePos = Nest::Events::getCursorPos();
    static auto resolution = Nest::Application::get()->getWindow()->getSize();
    resolution = Nest::Application::get()->getWindow()->getSize();
    static auto viewMatrix = camera->getViewMatrix();
    viewMatrix = camera->getViewMatrix();
    static auto projectionMatrix = camera->getProjectionMatrix();
    projectionMatrix = camera->getProjectionMatrix();
    static auto cameraPos = camera->getPosition();
    cameraPos = camera->getPosition();
    NestRen::setShader(m_shader);
    NestRen::setUniform(m_shader, "iTimeVec4", &time, NestRen::UniformType::Vec4); /// float
    NestRen::setUniform(m_shader, "iResolutionVec4", &resolution, NestRen::UniformType::Vec4); /// vec2
    NestRen::setUniform(m_shader, "iMouseVec4", &mousePos, NestRen::UniformType::Vec4); /// vec2
    NestRen::setUniform(m_shader, "iCameraPosVec4", &cameraPos, NestRen::UniformType::Vec4); /// vec4
    NestRen::setUniform(m_shader, "u_model", &m_model, NestRen::UniformType::Mat4); /// mat4
    NestRen::setUniform(m_shader, "u_view", &viewMatrix, NestRen::UniformType::Mat4); /// mat4
    NestRen::setUniform(m_shader, "u_projection", &projectionMatrix, NestRen::UniformType::Mat4); /// mat4
    static int slot = 0;
    NestRen::setTexture(m_texture, slot);
    NestRen::setUniform(m_shader, "iTexture", &slot, NestRen::UniformType::Sampler);
    NestRen::setIndexBuffer(m_indexBuffer, 0, 36);
    NestRen::setVertexBuffer(m_vertexBuffer);
    NestRen::submit(0);
}

void CubeRenderer::onImGuiRender() {

}

void CubeRenderer::onDetach() {
    deleteVertexBuffer(m_vertexBuffer);
    deleteIndexBuffer(m_indexBuffer);
    deleteProgram(m_shader);
    deleteTexture(m_texture);
}

void CubeRenderer::rotateX(float degrees) {
    m_model = glm::rotate(m_model, glm::radians(degrees), glm::vec3(1.0f, 0.0f, 0.0f));
}

void CubeRenderer::rotateY(float degrees) {
    m_model = glm::rotate(m_model, glm::radians(degrees), glm::vec3(0.0f, 1.0f, 0.0f));
}

void CubeRenderer::rotateZ(float degrees) {
    m_model = glm::rotate(m_model, glm::radians(degrees), glm::vec3(0.0f, 0.0f, 1.0f));
}

