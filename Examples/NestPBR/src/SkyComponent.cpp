#include "SkyComponent.hpp"

SkyComponent::~SkyComponent() {
    delete m_shaderCubeMap;
}

void SkyComponent::init(const std::array<std::string, 6> &skyTextureAsset) {
    m_shaderCubeMap = new Shader;
    m_shaderCubeMap->create("Shaders/vstSky.glsl", "Shaders/fstSky.glsl");

    SkyVertex vertices[8] = {
        SkyVertex(-1.0f, -1.0f, -1.0f), // 0
        SkyVertex(1.0f, -1.0f, -1.0f),  // 1
        SkyVertex(-1.0f, 1.0f, -1.0f),  // 2
        SkyVertex(1.0f, 1.0f, -1.0f),   // 3

        SkyVertex(-1.0f, -1.0f, 1.0f), // 4
        SkyVertex(1.0f, -1.0f, 1.0f),  // 5
        SkyVertex(-1.0f, 1.0f, 1.0f),  // 6
        SkyVertex(1.0f, 1.0f, 1.0f),   // 7
    };

    uint32_t indices[36] = {
        4, 6, 7, 7, 5, 4, // Front
        0, 2, 3, 3, 1, 0, // Back
        2, 3, 7, 7, 6, 2, // Top
        0, 1, 5, 5, 4, 0, // Bottom
        0, 2, 6, 6, 4, 0, // Left
        1, 5, 7, 7, 3, 1  // Right
    };

    VertexBufferLayout layout;
    layout.pushVec3F();
    m_mesh.create(layout, (float *)vertices, 8 * 3, indices, 36);
    m_cubeMap.create(skyTextureAsset);
}

CubeMap SkyComponent::getCubeMap() {
    return m_cubeMap;
}

void SkyComponent::draw() {
    auto camera = Application::getInstance()->getCamera();
    m_shaderCubeMap->use();
    m_shaderCubeMap->setFloat("iTime", Application::getInstance()->getWindow()->getTime());
    m_shaderCubeMap->setVec2("iMouse", Events::getCursorPos());
    m_shaderCubeMap->setVec2("iResolution", Application::getInstance()->getWindow()->getSize());
    m_shaderCubeMap->setMat4("u_view", camera->getSkyViewMatrix());
    m_shaderCubeMap->setMat4("u_projection", camera->getProjectionMatrix());
    m_cubeMap.bind(0);
    m_shaderCubeMap->setInt("iSky", 0);
    glDisable(GL_DEPTH_TEST);
    m_mesh.draw();
    glEnable(GL_DEPTH_TEST);
}