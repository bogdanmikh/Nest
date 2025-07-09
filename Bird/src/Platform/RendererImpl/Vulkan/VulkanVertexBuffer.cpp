//
// Created by Admin on 11.02.2022.
//

#include "VulkanVertexBuffer.hpp"
#include "VulkanBase.hpp"

namespace Bird {

VulkanVertexBuffer::VulkanVertexBuffer()
    : m_isDynamic(false)
    , m_id(-1)
    , m_layoutHandle(BIRD_INVALID_HANDLE) {}

void VulkanVertexBuffer::terminate() {
    m_vulkanBuffer.terminate();
}

void VulkanVertexBuffer::create(void *data, uint32_t size, bool isDynamic) {
    m_vulkanBuffer.m_delegate = m_delegate;
    m_vulkanBuffer.create(data, size, BufferType::Vertex, isDynamic);
}

void VulkanVertexBuffer::update(void *data, uint32_t size) {
    m_vulkanBuffer.update(data, size);
}

void VulkanVertexBuffer::bind() {
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(m_delegate->getCommandBuffer(), 0, 1, &m_vulkanBuffer.m_buffer, offsets);
}

void VulkanVertexBuffer::unbind() {}

void VulkanVertexBuffer::setLayoutHandle(VertexLayoutHandle layoutHandle) {
    m_layoutHandle = layoutHandle;
}

VertexLayoutHandle VulkanVertexBuffer::getLayoutHandle() {
    return m_layoutHandle;
}

} // namespace Bird
