#include "SphereRenderer.hpp"

void SphereRenderer::onAttach() {
    m_camera = Nest::Application::get()->getWorldCamera();

    m_countIndices = 0;
    metallic = 0.5;

    Bird::setViewClear(0, 0x3D75C9FF);

    std::vector<VertexFigure> vertices;
    std::vector<uint32_t> indices;
    int sectorCount = 36, stackCount = 18;
    float radius = 1.0f;
    const float PI = 3.14159265358979323846f;
    float x, y, z, xy;                           // position
    float nx, ny, nz, lengthInv = 1.0f / radius; // normal
    float s, t;                                  // texcoords

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
        Foundation::Memory::copying(vertices.data(), sizeof(VertexFigure) * vertices.size());
    Foundation::Memory indicesMemory =
        Foundation::Memory::copying(indices.data(), sizeof(uint32_t) * m_countIndices);

    Bird::VertexBufferLayoutData layoutData;
    layoutData.pushVec3();
    layoutData.pushVec2();
    layoutData.pushVec3();
    Bird::VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);
    m_vertexBuffer =
        createVertexBuffer(verticesMemory, vertices.size() * sizeof(VertexFigure), vertexLayout);
    m_indexBuffer =
        createIndexBuffer(indicesMemory, Bird::BufferElementType::UnsignedInt, m_countIndices);
}

void SphereRenderer::onUpdate(double deltaTime) {
    time = Nest::Application::get()->getWindow()->getTime();
    mousePos = {Nest::Input::getMousePositionX(), Nest::Input::getMousePositionY()};
    resolution = Nest::Application::get()->getWindow()->getSize();

    projViewMtx = m_camera->getProjectionMatrix() * m_camera->getViewMatrix();

    cameraPos = m_camera->getPosition();

    model = m_transformComponent.getTransform();

    static auto color = glm::vec3(1., 1., 1.);

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
                     Bird::UniformType::Vec4);                            /// vec4
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

    Bird::setIndexBuffer(m_indexBuffer, 0, m_countIndices);
    Bird::setVertexBuffer(m_vertexBuffer);
    Bird::submit(0);
}

void SphereRenderer::onImGuiRender() {}

void SphereRenderer::onDetach() {
    deleteVertexBuffer(m_vertexBuffer);
    deleteIndexBuffer(m_indexBuffer);
    deleteProgram(m_shader);
    deleteTexture(m_texture);
}