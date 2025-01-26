//
// Created by Bogdan on 23.08.2024.
//

#include "Player.hpp"

void Player::onAttach() {
    using namespace Bird;

    Bird::setViewClear(0, 0x3D75C9FF);

    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgram("Shaders/vstCircle.glsl", "Shaders/fstCircle.glsl");
    m_shader = createProgram(programAsset.getBirdProgramCreate());

    Nest::TextureAsset textureAsset = Nest::AssetLoader::loadTexture("Textures/Circle.png");

    TextureCreate textureCreate = textureAsset.getBirdTextureCreate();
    textureCreate.m_numMips = 4;
    textureCreate.m_minFiltering = Bird::NEAREST_MIPMAP_LINEAR;
    textureCreate.m_magFiltering = Bird::NEAREST;
    m_texture = createTexture(textureCreate);

    VertexCircle vertices[4]{
        VertexCircle(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        VertexCircle(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        VertexCircle(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),
        VertexCircle(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f)
    };

    uint32_t indices[6] = {0, 1, 2, 2, 3, 0};

    m_countIndices = 6;
    Foundation::Memory verticesMemory =
        Foundation::Memory::copying(&vertices, sizeof(VertexCircle) * 4);
    Foundation::Memory indicesMemory =
        Foundation::Memory::copying(&indices, sizeof(uint32_t) * m_countIndices);

    VertexBufferLayoutData layoutData;
    layoutData.pushVec3();
    layoutData.pushVec2();
    layoutData.pushVec3();
    VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);
    m_vertexBuffer = createVertexBuffer(verticesMemory, 4 * sizeof(VertexCircle), vertexLayout);
    m_indexBuffer =
        createIndexBuffer(indicesMemory, BufferElementType::UnsignedInt, m_countIndices);
}

void Player::onUpdate(double deltaTime) {
    static auto camera = Nest::Application::get()->getWorldCamera();
    static double time;
    time = Nest::Application::get()->getWindow()->getTime();
    static glm::vec2 mousePos;
    mousePos = {Nest::Input::getMousePositionX(), Nest::Input::getMousePositionY()};
    static glm::vec2 resolution;
    resolution = Nest::Application::get()->getWindow()->getSize();

    static glm::mat4 projViewMtx;
    projViewMtx = camera->getProjectionMatrix() * camera->getViewMatrix();

    static glm::vec3 cameraPos = camera->getPosition();
    cameraPos = camera->getPosition();

    static auto model = m_transformComponent.getTransform();
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
    Bird::setUniform(m_shader, "iColorVec4", &m_color,
                     Bird::UniformType::Vec4); /// vec3
    static int slot = 0;
    Bird::setTexture(m_texture, slot);
    Bird::setUniform(m_shader, "iTexture", &slot, Bird::UniformType::Sampler);
    Bird::setIndexBuffer(m_indexBuffer, 0, 36);
    Bird::setVertexBuffer(m_vertexBuffer);
    Bird::submit(0);
}

void Player::onImGuiRender() {}

void Player::onDetach() {
    deleteVertexBuffer(m_vertexBuffer);
    deleteIndexBuffer(m_indexBuffer);
    deleteProgram(m_shader);
}

Nest::TransformComponent &Player::getTransform() {
    return m_transformComponent;
}

void Player::setColor(glm::vec3 color) {
    m_color = color;
}

void Player::onMove(double deltaTime) {
    using namespace Nest;

    if (Input::isKeyPressed(Key::W)) {
        m_transformComponent.translate({0., speed * deltaTime, 0.});
    }

    if (Input::isKeyPressed(Key::S)) {
        m_transformComponent.translate({0., -speed * deltaTime, 0.});
    }

    if (Input::isKeyPressed(Key::A)) {
        m_transformComponent.translate({-speed * deltaTime, 0., 0.});
    }

    if (Input::isKeyPressed(Key::D)) {
        m_transformComponent.translate({speed * deltaTime, 0., 0.});
    }

    float borderX = 32.9, borderY = 21.5;
    auto pos = m_transformComponent.getPosition();
    pos.x = std::max(-borderX, pos.x);
    pos.x = std::min(borderX, pos.x);

    pos.y = std::max(-borderY, pos.y);
    pos.y = std::min(borderY, pos.y);
    m_transformComponent.setPosition(pos);
}
