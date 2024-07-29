#include "Nest/Renderer/VertexBuffer.hpp"
#include "glad/glad.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
    create(data, size);
}

void VertexBuffer::create(const void *data, unsigned int size) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::destroy() {
    if (m_RendererID) {
        glDeleteBuffers(1, &m_RendererID);
    }
    m_RendererID = 0;
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}
void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
