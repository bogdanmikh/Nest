#pragma once

#include <Nest.hpp>
/*
class Cross final: public GameObject {
public:
    ~Cross() {
        delete m_mesh;
        delete m_shader;
    }
    void init() {
//        m_shader = new OpenGLShader("Nest/res/Shaders/crossvst.glsl",
"Nest/res/Shaders/crossfst.glsl");
//        m_shader->use();
        Vertex vertex[4];
        vertex[0].Position = glm::vec3(-0.1, -0.1, 0);
        vertex[1].Position = glm::vec3(0.1, -0.1, 0);
        vertex[2].Position = glm::vec3(0.1, 0.1, 0);
        vertex[3].Position = glm::vec3(-0.1, 0.1, 0);
        uint32_t indices[6] {
                0, 1, 2, 2, 3,0
        };
        m_mesh = new Mesh(vertex, 4, indices, 6);
        m_mesh->addTexture("Examples/NestCraft/res/Aim.png");
    }
    void update(double deltaTime) {        m_mesh->draw();
    }
    void draw() {}
private:
    Mesh* m_mesh;
    OpenGLShader* m_shader;
};*/