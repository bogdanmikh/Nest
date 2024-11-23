//
// Created by Bogdan
//

#include "VulkanVertexBuffer.hpp"

#include "VulkanBase.hpp"

namespace Bird {

VulkanVertexBuffer::VulkanVertexBuffer()
    : m_isDynamic(false)
    , m_id(-1)
    , m_layoutHandle(BIRD_INVALID_HANDLE) {}

void VulkanVertexBuffer::terminate() {
    NEST_ASSERT(m_id != -1, "VERTEX BUFFER ALREADY DELETED");
    GL_CALL(glDeleteBuffers(1, &m_id));
    m_id = -1;
}

void VulkanVertexBuffer::create(void *data, uint32_t size, bool isDynamic) {
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

void VulkanVertexBuffer::update(void *data, uint32_t size) {
    NEST_ASSERT(m_id != -1, "VERTEX BUFFER NOT VALID");
    NEST_ASSERT(m_isDynamic != false, "Невозможно обновить статичный буфер");
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VulkanVertexBuffer::bind() {
    NEST_ASSERT(m_id != -1, "VERTEX BUFFER NOT VALID");
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void VulkanVertexBuffer::unbind() {
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VulkanVertexBuffer::setLayoutHandle(VertexLayoutHandle layoutHandle) {
    m_layoutHandle = layoutHandle;
}

VertexLayoutHandle VulkanVertexBuffer::getLayoutHandle() {
    return m_layoutHandle;
}

} // namespace Bird
