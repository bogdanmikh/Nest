#pragma once

#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"

class Mesh {
public:
    Mesh() = default;
    Mesh(
        Vertex *vertices,
        unsigned int verticesCount,
        unsigned int *indices,
        unsigned int indicesCount
    );
    Mesh(
        const VertexBufferLayout &layout,
        float *vertices,
        unsigned int verticesCount,
        unsigned int *indices,
        unsigned int indicesCount
    );
    void create(
        const VertexBufferLayout &layout,
        float *vertices,
        unsigned int verticesCount,
        unsigned int *indices,
        unsigned int indicesCount
    );
    void create(
        Vertex *vertices,
        unsigned int verticesCount,
        unsigned int *indices,
        unsigned int indicesCount
    );
    ~Mesh();
    void draw();
    void addTexture(const std::string &texturePath);

private:
    VertexArray *va;
    IndexBuffer *ib;
    VertexBuffer *vb;
    std::vector<Texture *> textures;
    uint32_t indicesCount;
};