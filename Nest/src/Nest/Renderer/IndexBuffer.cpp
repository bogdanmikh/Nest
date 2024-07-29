#include "Nest/Renderer/IndexBuffer.hpp"
#include "glad/glad.h"

IndexBuffer::IndexBuffer(unsigned int *data, unsigned int count) {
    create(data, count);
}

void IndexBuffer::create(unsigned int *data, unsigned int count) {
    m_Size = count;
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void IndexBuffer::destroy() {
    if (m_RendererID) {
        glDeleteBuffers(1, &m_RendererID);
    }
    m_RendererID = 0;
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
unsigned int IndexBuffer::getSize() const {
    return m_Size;
}
