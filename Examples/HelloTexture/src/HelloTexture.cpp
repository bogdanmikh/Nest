//
// Created by admin on 26.12.2023.
//

#include "HelloTexture.hpp"

void HelloTexture::start(Window *window) {
    const int vertex = 16;
    const float pos[vertex] {
        //     x   y           u   v
            0.0f, 0.0f,      0.0f, 0.0f,   // 0
            1.0f, 0.0f,      1.0f, 0.0f,   // 1
            1.0f, 1.0f,      1.0f, 1.0f,   // 2
            0.0f, 1.0f,      0.0f, 1.0f    // 3
    };

    const int sizeTriangle = 6;
    unsigned int indices[sizeTriangle] = {
            0, 1, 2,
            0, 2, 3
    };

    VertexBuffer vertexBuffer(pos, vertex * sizeof(float));

    VertexArray vertexArray;

    VertexBufferLayout vertexBufferLayout;
    vertexBufferLayout.pushVec2F(1);
    vertexBufferLayout.pushVec2F(1);
    vertexArray.addBuffer(vertexBuffer, vertexBufferLayout);

    IndexBuffer indexBuffer(indices, sizeTriangle);

    auto *shader = new Shader("C:/Projects/Nest/Nest/res/Shaders/vst.glsl",
                              "C:/Projects/Nest/Nest/res/Shaders/fst.glsl");

    Texture texture("C:/Projects/Nest/Examples/HelloTexture/res/textures/Loh.png");
    texture.bind();

    shader->use();

    Renderer::init();

    
    while (!window->shouldClose() && !window->isKeyPressed(Key::ESCAPE)) {
        Renderer::clear();

        shader->use();

        shader->setFloat("u_time", window->getTime());
        shader->setVec2("u_mouse", window->getCursorPos());
        shader->setVec2("u_resolution", window->getSize());

        Renderer::drawIndexed(6);

        window->swapBuffers();

        window->pollEvents();
    }
    delete shader;
}
