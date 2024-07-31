#include "Nest/Renderer/OpenGL/VertexArray.hpp"
#include "Nest/Renderer/OpenGL/ErrorsOpenGL.hpp"

VertexArray::VertexArray() {
    GL_CALL(glGenVertexArrays(1, &m_rendererID));
}

VertexArray::~VertexArray() {
    GL_CALL(glDeleteVertexArrays(1, &m_rendererID));
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) const {
    bind();
    vb.bind();
    const auto &elements = layout.getElements();
    long offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto &element = elements[i];
        GL_CALL(glEnableVertexAttribArray(i));
        GL_CALL(glVertexAttribPointer(
            i,
            element.count,
            element.type,
            element.normalized,
            layout.getStride(),
            (const void *)offset
        ));
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::bind() const {
    GL_CALL(glBindVertexArray(m_rendererID));
}

void VertexArray::unbind() const {
    GL_CALL(glBindVertexArray(0));
}
