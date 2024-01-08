#include "Nest/NestObjects/Shapes/Cube.hpp"

Cube::Cube() : mPosition(0.f), mRotation(0.f), mSize(0.5) {}

void Cube::init() {
    auto vertices = new Vertex[24]{
            // Front
            Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f), // 0
            Vertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f),  // 1
            Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f),   // 2
            Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f),  // 3
            // Back
            Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f), // 4
            Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f),  // 5
            Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f),   // 6
            Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f),  // 7
            // Top
            Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f), // 8
            Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 1.0f),  // 11
            Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f),   // 10
            Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f),  // 9
            // Bottom
            Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f), // 12
            Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),  // 13
            Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f),   // 14
            Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f),  // 15
            // Left
            Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f), // 16
            Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),  // 17
            Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f),   // 18
            Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f),  // 19
            // Right
            Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f), // 20
            Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f),  // 23
            Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f),   // 22
            Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f)   // 21
    };

    // clang-format off
    auto indices = new uint32_t[36]{
            0, 1, 2, 2, 3, 0,       // Front
            4, 5, 6, 6, 7, 4,       // Back
            8, 9, 10, 10, 11, 8,    // Top
            12, 13, 14, 14, 15, 12, // Bottom
            16, 17, 18,  18, 19, 16, // Left
            20, 21, 22, 22, 23, 20  // Right
    };
    mesh = new Mesh(vertices, 24, indices, 36);
    Renderer::checkForErrors();

    delete[] vertices;
    delete[] indices;
}

void Cube::updateModelMatrix() {
    mShader->use();
    glm::mat4 model = glm::translate(glm::mat4(1.f), mPosition);
    model = glm::scale(model, mSize);
    model = glm::rotate(model, mRotation.x, glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, mRotation.y, glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, mRotation.z, glm::vec3(0.f, 0.f, 1.f));
    mShader->setMat4("u_model", model);
    Renderer::checkForErrors();
}

void Cube::draw() {
    mesh->draw();
    updateModelMatrix();
    Renderer::checkForErrors();
}