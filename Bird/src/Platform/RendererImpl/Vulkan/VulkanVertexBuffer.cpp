//
// Created by Admin on 11.02.2022.
//

#include "VulkanVertexBuffer.hpp"

namespace Bird {

VulkanVertexBuffer::VulkanVertexBuffer()
    : m_isDynamic(false)
    , m_id(-1)
    , m_layoutHandle(BIRD_INVALID_HANDLE) {}

void VulkanVertexBuffer::terminate() {}

void VulkanVertexBuffer::create(void *data, uint32_t size, bool isDynamic) {}

void VulkanVertexBuffer::update(void *data, uint32_t size) {}

void VulkanVertexBuffer::bind() {}

void VulkanVertexBuffer::unbind() {}

void VulkanVertexBuffer::setLayoutHandle(VertexLayoutHandle layoutHandle) {
    m_layoutHandle = layoutHandle;
}

VertexLayoutHandle VulkanVertexBuffer::getLayoutHandle() {
    return m_layoutHandle;
}

} // namespace Bird
