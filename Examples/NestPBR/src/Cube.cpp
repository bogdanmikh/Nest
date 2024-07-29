#include "Cube.hpp"
#include "glm/ext/matrix_transform.hpp"

Cube::~Cube() {
    m_texture.destroy();
    m_cubeMap.destroy();
    m_shaderCube.destroy();
}

void Cube::init(const CreateInfo& createInfo) {
    m_createInfo = createInfo;
    m_model = glm::mat4(1);
    m_model = glm::translate(m_model, m_createInfo.position);
    if (std::filesystem::exists(m_createInfo.pathToGeometryShader)) {
        m_shaderCube.create(
            m_createInfo.pathToVertexShader.c_str(), m_createInfo.pathToFragmentShader.c_str()
                                                         , m_createInfo.pathToGeometryShader.c_str()
        );
    } else {
        m_shaderCube.create(
            m_createInfo.pathToVertexShader.c_str(), m_createInfo.pathToFragmentShader.c_str()
        );
    }
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

    uint32_t indices[36] = {
       0, 1, 2, 2, 3, 0,       // Front
       4, 5, 6, 6, 7, 4,       // Back
       8, 9, 10, 10, 11, 8,    // Top
       12, 13, 14, 14, 15, 12, // Bottom
       16, 17, 18, 18, 19, 16, // Left
       20, 21, 22, 22, 23, 20  // Right
    };
    // clang-format on

    VertexBufferLayout layout;
    layout.pushVec3F();
    layout.pushVec2F();
    layout.pushVec3F();
    m_mesh.create(layout, (float *)vertices, 24 * 8, indices, 36);
    if (m_createInfo.useTexture) {
        m_texture.create(m_createInfo.pathToTexture);
    }
    if (m_createInfo.useCubeMap) {
        m_cubeMap.create(m_createInfo.skyTextureAsset);
    }
}

void Cube::rotateX(float degrees) {
    m_model = glm::rotate(m_model, glm::radians(degrees), glm::vec3(1.0f, 0.0f, 0.0f));
}

void Cube::rotateY(float degrees) {
    m_model = glm::rotate(m_model, glm::radians(degrees), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Cube::rotateZ(float degrees) {
    m_model = glm::rotate(m_model, glm::radians(degrees), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Cube::draw(double deltaTime) {
    auto camera = Application::getInstance()->getCamera();
    m_shaderCube.use();
    m_shaderCube.setFloat("iTime", Application::getInstance()->getWindow()->getTime());
    m_shaderCube.setFloat("time", Application::getInstance()->getWindow()->getTime());
    m_shaderCube.setVec2("iMouse", Events::getCursorPos());
    m_shaderCube.setVec2("iResolution", Application::getInstance()->getWindow()->getSize());
    m_shaderCube.setMat4("u_model", m_model);
    m_shaderCube.setMat4("u_view", camera->getViewMatrix());
    m_shaderCube.setMat4("u_projection", camera->getProjectionMatrix());
    m_shaderCube.setVec3("cameraPos", camera->getPosition());
    if (m_createInfo.useTexture) {
        m_texture.bind(0);
        m_shaderCube.setInt(m_createInfo.nameTexture, 0);
    }
    if (m_createInfo.useCubeMap) {
        m_cubeMap.bind(1);
        m_shaderCube.setInt(m_createInfo.nameCubeMap, 1);
    }
    m_mesh.draw();
}
