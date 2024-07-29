#include "Nest/Renderer/Mesh.hpp"
#include "Nest/Logger/Logger.hpp"

#include <iostream>
#include <filesystem>

Mesh::Mesh(
    Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount
) {
    create(vertices, verticesCount, indices, indicesCount);
}

Mesh::Mesh(
    const VertexBufferLayout &layout,
    float *vertices,
    unsigned int verticesCount,
    unsigned int *indices,
    unsigned int indicesCount
) {
    create(layout, vertices, verticesCount, indices, indicesCount);
}

Mesh::~Mesh() {
    for (int i = 0; i < textures.size(); i++) {
        if (textures[i] != nullptr) {
            delete textures[i];
        }
    }
    delete ib;
    delete vb;
    delete va;
}

void Mesh::create(
    const VertexBufferLayout &layout,
    float *vertices,
    unsigned int verticesCount,
    unsigned int *indices,
    unsigned int indicesCount
) {
    ib = new IndexBuffer(indices, indicesCount);
    vb = new VertexBuffer(vertices, sizeof(float) * verticesCount);
    this->indicesCount = indicesCount;
    va = new VertexArray;
    va->addBuffer(*vb, layout);
}

void Mesh::create(
    Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount
) {
    ib = new IndexBuffer(indices, indicesCount);
    vb = new VertexBuffer(vertices, sizeof(Vertex) * verticesCount);
    this->indicesCount = indicesCount;

    VertexBufferLayout layout;
    layout.pushVertex();
    va = new VertexArray;
    va->addBuffer(*vb, layout);
}

void Mesh::draw() {
    for (int i = 0; i < textures.size(); i++) {
        if (textures[i] == nullptr) {
            LOG_ERROR("Texture is null");
            return;
        }
        auto &texture = textures[i];
        texture->bind(i);
    }
    if (va == nullptr || ib == nullptr) {
        LOG_ERROR("Vertex Array or Index Buffer not init");
        return;
    }
    va->bind();
    ib->bind();
    Renderer::drawIndexed(indicesCount);
    Renderer::checkForErrors();
}

void Mesh::addTexture(const std::string &texturePath) {
    auto texture = new Texture(texturePath);
    textures.push_back(texture);
}