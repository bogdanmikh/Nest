#pragma once

#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"

class Mesh {
public:
    Mesh(Vertex *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount);
    Mesh(float *vertices, unsigned int verticesCount, unsigned int *indices, unsigned int indicesCount);
    ~Mesh();
    void draw();
    void addTexture(const std::string& texturePath);
    void update(Vertex *vertices, unsigned int verticesCount);
private:
    VertexArray *va;
    IndexBuffer *ib;
    VertexBuffer *vb;
    std::vector<Texture*> textures;
    uint32_t indices;
};