//
// Created by Admin on 11.02.2022.
//

#include "VulkanVertexBuffer.hpp"
#include "VulkanBase.hpp"

namespace Bird {

VulkanVertexBuffer::VulkanVertexBuffer()
    : m_isDynamic(false)
    , m_layoutHandle(BIRD_INVALID_HANDLE) {}

void VulkanVertexBuffer::create(void *data, uint32_t size, bool isDynamic) {
    m_buffer.m_delegate = m_delegate;
    m_buffer.create(data, size, BufferType::Vertex, isDynamic);
}

void VulkanVertexBuffer::terminate() {
    m_buffer.terminate();
}

void VulkanVertexBuffer::update(void *data, uint32_t size) {
    m_buffer.update(data, size);
}

void VulkanVertexBuffer::bind() {
    NEST_ASSERT(m_buffer.isValid(), "VERTEX BUFFER ALREADY DELETED");
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(m_delegate->getCommandBuffer(), 0, 1, &m_buffer.m_buffer, offsets);
}

void VulkanVertexBuffer::setLayoutHandle(VertexLayoutHandle layoutHandle) {
    m_layoutHandle = layoutHandle;
}

VertexLayoutHandle VulkanVertexBuffer::getLayoutHandle() {
    return m_layoutHandle;
}

} // namespace Bird
