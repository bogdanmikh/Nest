//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Bird/VertexBufferLayoutData.hpp"
#include "VulkanBuffer.hpp"

namespace Bird {

class VulkanIndexBuffer {
public:
    VulkanIndexBuffer();
    void create(void *indices, BufferElementType elementType, size_t count, bool isDynamic);
    void terminate();
    void update(void *indices, size_t count);
    void bind() const;
    VulkanBufferDelegate *m_delegate;

private:
    uint32_t m_count;
    size_t m_elementSize;
    bool m_isDynamic;
    VulkanBuffer m_buffer;
    VkDeviceSize m_size;
    VkIndexType m_indexType;
};

} // namespace Bird
