#include "Nest/NestRenderer/Mesh.hpp"

#include <iostream>

Mesh::Mesh(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount)
        : ib(new IndexBuffer(indices, indicesCount))
        , vb(new VertexBuffer(vertices, sizeof(Vertex) * verticesCount))
        , indices(indicesCount) {
    VertexBufferLayout layout;
    layout.pushVertex(1);
    va = new VertexArray;
    va->addBuffer(*vb, layout);
}

Mesh::~Mesh() {
    for (int i = 0; i < textures.size(); i++) {
        delete textures[i];
    }
    delete ib;
    delete vb;
    delete va;
}

void Mesh::draw() {
    for (int i = 0; i < textures.size(); i++) {
        auto &texture = textures[i];
        texture->bind(i);
    }
    va->bind();
    ib->bind();
    Renderer::drawIndexed(indices);
}

void Mesh::addTexture(const std::string& texturePath) {
    auto texture = new Texture(texturePath);
    textures.push_back(texture);
}


