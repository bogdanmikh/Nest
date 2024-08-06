//
// Created by Admin on 11.02.2022.
//

#include "OpenGLIndexBuffer.hpp"

#include "OpenGLBase.hpp"

namespace Bird {

OpenGLIndexBuffer::OpenGLIndexBuffer()
    : m_id(-1)
    , m_count(0)
    , m_elementType(0)
    , m_elementSize(0)
    , m_isDynamic(false) {}

void OpenGLIndexBuffer::create(
    void *indices, BufferElementType elementType, size_t count, bool isDynamic
) {
    NEST_ASSERT(m_id == -1, "INDEX BUFFER ALREADY CREATED");
    m_isDynamic = isDynamic;
    m_count = count;

    if (elementType == BufferElementType::UnsignedByte) {
        m_elementType = GL_UNSIGNED_BYTE;
        m_elementSize = sizeof(unsigned char);
    } else if (elementType == BufferElementType::UnsignedShort) {
        m_elementType = GL_UNSIGNED_SHORT;
        m_elementSize = sizeof(unsigned short);
    } else {
        m_elementType = GL_UNSIGNED_INT;
        m_elementSize = sizeof(unsigned int);
    }
    GL_CALL(glGenBuffers(1, &m_id));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    if (indices != nullptr) {
        GL_CALL(glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            count * m_elementSize,
            indices,
            isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW
        ));
    }
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void OpenGLIndexBuffer::terminate() {
    NEST_ASSERT(m_id != -1, "INDEX BUFFER ALREADY DELETED");
    GL_CALL(glDeleteBuffers(1, &m_id));
    m_id = -1;
}

void OpenGLIndexBuffer::bind() const {
    NEST_ASSERT(m_id != -1, "INDEX BUFFER NOT VALID");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void OpenGLIndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t OpenGLIndexBuffer::getCount() const {
    NEST_ASSERT(m_id != -1, "INDEX BUFFER NOT VALID");
    return m_count;
}

void OpenGLIndexBuffer::update(void *indices, size_t count) {
    NEST_ASSERT(m_isDynamic != false, "Невозможно обновить статичный буфер");
    m_count = count;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * m_elementSize, indices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace Bird
