//
// Created by Admin on 11.02.2022.
//

#include "VulkanIndexBuffer.hpp"

namespace Bird {

VulkanIndexBuffer::VulkanIndexBuffer()
    : m_isDynamic(false) {}

void VulkanIndexBuffer::create(
    void *indices, BufferElementType elementType, size_t count, bool isDynamic
) {
    m_isDynamic = isDynamic;
    m_count = count;

    if (elementType == BufferElementType::UnsignedByte) {
        m_indexType = VkIndexType::VK_INDEX_TYPE_UINT8_KHR;
        m_elementSize = sizeof(unsigned char);
    } else if (elementType == BufferElementType::UnsignedShort) {
        m_indexType = VkIndexType::VK_INDEX_TYPE_UINT16;
        m_elementSize = sizeof(unsigned short);
    } else {
        m_indexType = VkIndexType::VK_INDEX_TYPE_UINT32;
        m_elementSize = sizeof(unsigned int);
    }
    m_size = m_elementSize * m_count;

    m_buffer.m_delegate = m_delegate;
    m_buffer.create(indices, m_size, BufferType::Index, m_isDynamic);
}

void VulkanIndexBuffer::terminate() {
    m_buffer.terminate();
}

void VulkanIndexBuffer::bind() const {
    NEST_ASSERT(m_buffer.isValid(), "VERTEX BUFFER ALREADY DELETED");
    vkCmdBindIndexBuffer(m_delegate->getCommandBuffer(), m_buffer.m_buffer, 0, m_indexType);
}

void VulkanIndexBuffer::update(void *indices, size_t count) {
    m_buffer.update(indices, count);
}

} // namespace Bird
