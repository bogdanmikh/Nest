#include "Nest/NestRenderer/Mesh.hpp"
#include "Nest/AMSTL/LocalPath.hpp"

#include <iostream>
#include <filesystem>

Mesh::Mesh(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount)
        : ib(new IndexBuffer(indices, indicesCount))
        , vb(new VertexBuffer(vertices, sizeof(Vertex) * verticesCount))
        , indices(indicesCount) {
    VertexBufferLayout layout;
    layout.pushVertex();
    va = new VertexArray;
    va->addBuffer(*vb, layout);
}

Mesh::Mesh(float *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount)
        : ib(new IndexBuffer(indices, indicesCount))
        , vb(new VertexBuffer(vertices, sizeof(float) * verticesCount))
        , indices(indicesCount) {
    VertexBufferLayout layout;
    layout.pushVec3F();
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
    Renderer::checkForErrors();
}

void Mesh::update(Vertex *vertices, unsigned int verticesCount) {

}

void Mesh::addTexture(const std::string& texturePath) {
    std::string texPath = (amstl::localPath + texturePath);
    auto texture = new Texture(texPath);
    textures.push_back(texture);
}