#include "SphereRenderer.hpp"

void SphereRenderer::onAttach() {
    using namespace NestRen;
    m_countIndices = 0;

    NestRen::setViewClear(0, 0x3D75C9FF);

    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vstSphere.glsl", "Shaders/fstSphere.glsl");
    m_shader = createProgram(programAsset.getNestRenProgramCreate());

    Nest::TextureAsset textureAsset = Nest::AssetLoader::loadTexture("Textures/Dubil6.png");

    TextureCreate textureCreate = textureAsset.getNestRenTextureCreate();
    textureCreate.m_numMips = 4;
    textureCreate.m_minFiltering = NestRen::NEAREST_MIPMAP_LINEAR;
    textureCreate.m_magFiltering = NestRen::NEAREST;
    m_texture = createTexture(textureCreate);

    std::vector<SphereVertex> vertices;
    std::vector<uint32_t> indices;
    int sectorCount = 36, stackCount = 18;
    float radius = 1.0f;
    const float PI = 3.14159265358979323846f;
    float x, y, z, xy;                           // позиция на сфере
    float nx, ny, nz, lengthInv = 1.0f / radius; // нормаль
    float s, t;                                  // текстурные координаты

    float sectorStep = 2.0f * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = PI / 2.0f - i * stackStep; // от pi/2 до -pi/2
        xy = radius * cosf(stackAngle);         // r * cos(u)
        z = radius * sinf(stackAngle);          // r * sin(u)

        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep; // от 0 до 2pi

            x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            s = (float)j / sectorCount;
            t = (float)i / stackCount;

            vertices.emplace_back(x, y, z, s, t, nx, ny, nz);
        }
    }

    int k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;
        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    // clang-format on
    m_countIndices = indices.size();
    Foundation::Memory verticesMemory =
        Foundation::Memory::copying(vertices.data(), sizeof(SphereVertex) * vertices.size());
    Foundation::Memory indicesMemory =
        Foundation::Memory::copying(indices.data(), sizeof(uint32_t) * m_countIndices);

    VertexBufferLayoutData layoutData;
    layoutData.pushVec3();
    layoutData.pushVec2();
    layoutData.pushVec3();
    VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);
    m_vertexBuffer =
        createVertexBuffer(verticesMemory, vertices.size() * sizeof(SphereVertex), vertexLayout);
    m_indexBuffer =
        createIndexBuffer(indicesMemory, BufferElementType::UnsignedInt, m_countIndices);
}

void SphereRenderer::onUpdate(double deltaTime) {
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
    static auto model = m_transformComponent.getTransform();
    model = m_transformComponent.getTransform();
    NestRen::setShader(m_shader);
    NestRen::setUniform(m_shader, "iTimeVec4", &time, NestRen::UniformType::Vec4); /// float
    NestRen::setUniform(
        m_shader, "iResolutionVec4", &resolution, NestRen::UniformType::Vec4
    );                                                                                  /// vec2
    NestRen::setUniform(m_shader, "iMouseVec4", &mousePos, NestRen::UniformType::Vec4); /// vec2
    NestRen::setUniform(
        m_shader, "iCameraPosVec4", &cameraPos, NestRen::UniformType::Vec4
    );                                                                                /// vec4
    NestRen::setUniform(m_shader, "u_model", &model, NestRen::UniformType::Mat4);     /// mat4
    NestRen::setUniform(m_shader, "u_view", &viewMatrix, NestRen::UniformType::Mat4); /// mat4
    NestRen::setUniform(
        m_shader, "u_projection", &projectionMatrix, NestRen::UniformType::Mat4
    ); /// mat4
    static int slot = 0;
    NestRen::setTexture(m_texture, slot);
    NestRen::setUniform(m_shader, "iTexture", &slot, NestRen::UniformType::Sampler);
    NestRen::setIndexBuffer(m_indexBuffer, 0, m_countIndices);
    NestRen::setVertexBuffer(m_vertexBuffer);
    NestRen::submit(0);
}

void SphereRenderer::onImGuiRender() {}

void SphereRenderer::onDetach() {
    deleteVertexBuffer(m_vertexBuffer);
    deleteIndexBuffer(m_indexBuffer);
    deleteProgram(m_shader);
    deleteTexture(m_texture);
}

Nest::TransformComponent &SphereRenderer::getTransform() {
    return m_transformComponent;
}