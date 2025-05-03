//
// Created by Admin on 11.02.2022.
//

#include "VulkanIndexBuffer.hpp"

namespace Bird {

VulkanIndexBuffer::VulkanIndexBuffer()
    : m_id(-1)
    , m_count(0)
    , m_elementType(0)
    , m_elementSize(0)
    , m_isDynamic(false) {}

void VulkanIndexBuffer::create(
    void *indices, BufferElementType elementType, size_t count, bool isDynamic
) {
    NEST_ASSERT(m_id == -1, "INDEX BUFFER ALREADY CREATED");
    m_isDynamic = isDynamic;
    m_count = count;

    if (elementType == BufferElementType::UnsignedByte) {

    } else if (elementType == BufferElementType::UnsignedShort) {

    } else {
    }
}

void VulkanIndexBuffer::terminate() {}

void VulkanIndexBuffer::bind() const {}

void VulkanIndexBuffer::unbind() const {}

uint32_t VulkanIndexBuffer::getCount() const {
    NEST_ASSERT(m_id != -1, "INDEX BUFFER NOT VALID");
    return m_count;
}

void VulkanIndexBuffer::update(void *indices, size_t count) {}

} // namespace Bird
