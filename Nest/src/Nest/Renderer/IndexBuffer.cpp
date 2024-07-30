#include "Nest/Renderer/IndexBuffer.hpp"
#include "Nest/Renderer/ErrorsOpenGL.hpp"

IndexBuffer::IndexBuffer(unsigned int *data, unsigned int count) {
    create(data, count);
}

void IndexBuffer::create(unsigned int *data, unsigned int count) {
    m_Size = count;
    GL_CALL(glGenBuffers(1, &m_RendererID));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void IndexBuffer::destroy() {
    if (m_RendererID) {
        GL_CALL(glDeleteBuffers(1, &m_RendererID));
    }
    m_RendererID = 0;
}

void IndexBuffer::bind() const {
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::unbind() const {
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
unsigned int IndexBuffer::getSize() const {
    return m_Size;
}
