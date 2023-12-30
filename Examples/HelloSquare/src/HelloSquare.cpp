//
// Created by admin on 28.12.2023.
//

#include "HelloSquare.hpp"

void HelloSquare::start(Window *window) {
    const unsigned int vertex = 8;
    const float pos[vertex] {
            0.0f, 0.0f, // 0
            1.0f, 1.0f, // 1
            1.0f, 0.0f, // 2
            0.0f, 1.0f  // 3
    };
    const unsigned int sizeTriangle = 6;
    unsigned int indices[sizeTriangle] = {
            0, 1, 2,
            0, 1, 3
    };
    VertexBuffer vertexBuffer(pos, vertex * sizeof(float));

    VertexArray vertexArray;
    VertexBufferLayout vertexBufferLayout;
    vertexBufferLayout.pushVec2F(1);
    vertexArray.addBuffer(vertexBuffer, vertexBufferLayout);

    IndexBuffer indexBuffer(indices, sizeTriangle);
    auto *shader = new Shader("C:/Projects/Nest/Nest/res/Shaders/vst.glsl",
                              "C:/Projects/Nest/Nest/res/Shaders/fst.glsl");

    shader->use();

    while (!window->shouldClose() && !window->isKeyPressed(Key::ESCAPE)) {
        Renderer::clear();
        shader->setFloat("u_time", window->getTime());
        shader->setVec2("u_mouse", window->getCursorPos());
        shader->setVec2("u_resolution", window->getSize());

        shader->use();

        Renderer::drawIndexed(6);

        window->swapBuffers();

        window->pollEvents();
    }
    delete shader;
}
