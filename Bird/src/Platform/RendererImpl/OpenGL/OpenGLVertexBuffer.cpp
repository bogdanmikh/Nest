//
// Created by Admin on 11.02.2022.
//

#include "OpenGLVertexBuffer.hpp"

#include "OpenGLBase.hpp"

namespace Bird {

OpenGLVertexBuffer::OpenGLVertexBuffer()
    : m_isDynamic(false)
    , m_id(-1)
    , m_layoutHandle(BIRD_INVALID_HANDLE) {}

void OpenGLVertexBuffer::terminate() {
    NEST_ASSERT(m_id != -1, "VERTEX BUFFER ALREADY DELETED");
    GL_CALL(glDeleteBuffers(1, &m_id));
    m_id = -1;
}

void OpenGLVertexBuffer::create(void *data, uint32_t size, bool isDynamic) {
    NEST_ASSERT(m_id == -1, "VERTEX BUFFER ALREADY CREATED");
    m_isDynamic = isDynamic;
    GL_CALL(glGenBuffers(1, &m_id));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    if (data != nullptr) {
        GL_CALL(
            glBufferData(GL_ARRAY_BUFFER, size, data, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW)
        );
    }
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    m_layoutHandle = BIRD_INVALID_HANDLE;
}

void OpenGLVertexBuffer::update(void *data, uint32_t size) {
    NEST_ASSERT(m_id != -1, "VERTEX BUFFER NOT VALID");
    NEST_ASSERT(m_isDynamic != false, "Невозможно обновить статичный буфер");
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void OpenGLVertexBuffer::bind() {
    NEST_ASSERT(m_id != -1, "VERTEX BUFFER NOT VALID");
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void OpenGLVertexBuffer::unbind() {
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void OpenGLVertexBuffer::setLayoutHandle(VertexLayoutHandle layoutHandle) {
    m_layoutHandle = layoutHandle;
}

VertexLayoutHandle OpenGLVertexBuffer::getLayoutHandle() {
    return m_layoutHandle;
}

} // namespace Bird
