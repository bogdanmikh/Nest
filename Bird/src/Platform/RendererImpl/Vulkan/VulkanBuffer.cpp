//
// Created by Bogdan on 07.07.2025.
//

#include "VulkanBuffer.hpp"
#include "Foundation/Assert.hpp"

VulkanBuffer::VulkanBuffer()
    : m_buffer(VK_NULL_HANDLE) {}

void VulkanBuffer::create(void *data, uint32_t size, BufferType type, bool isDynamic) {
    NEST_ASSERT(!isValid(), "VERTEX BUFFER ALREADY DELETED");
    m_allocatorCb = g_allocatorCb;
    m_device = *g_device;

    m_isDynamic = isDynamic;
    m_size = size;

    VkBufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = NULL;
    bufferCreateInfo.flags = 0;
    bufferCreateInfo.size = m_size;
    switch (type) {
        case BufferType::Vertex: {
            bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            break;
        }
        case BufferType::Index: {
            bufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            break;
        }
    }
    bufferCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.queueFamilyIndexCount = 0;
    bufferCreateInfo.pQueueFamilyIndices = NULL;

    VK_CHECK(vkCreateBuffer(m_device, &bufferCreateInfo, m_allocatorCb, &m_buffer));

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(m_device, m_buffer, &memoryRequirements);

    VK_CHECK(m_delegate->allocateMemory(
        &memoryRequirements,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_deviceMemory
    ));

    VK_CHECK(vkBindBufferMemory(m_device, m_buffer, m_deviceMemory, 0));

    if (!m_isDynamic) {
        update(data, size);
    }
}

void VulkanBuffer::update(void *data, uint32_t size) {
    NEST_ASSERT(isValid(), "VERTEX BUFFER ALREADY DELETED");
    VK_CHECK(vkMapMemory(m_device, m_deviceMemory, 0, size, 0, &data));
    vkUnmapMemory(m_device, m_deviceMemory);

    //    m_delegate->setMemoryBarrier(
    //        m_delegate->getCommandBuffer(),
    //        VK_PIPELINE_STAGE_TRANSFER_BIT,
    //        VK_PIPELINE_STAGE_TRANSFER_BIT
    //    );
}

void VulkanBuffer::terminate() {
    NEST_ASSERT(isValid(), "VERTEX BUFFER ALREADY DELETED");
    vkFreeMemory(m_device, m_deviceMemory, m_allocatorCb);
    vkDestroyBuffer(m_device, m_buffer, m_allocatorCb);
}

bool VulkanBuffer::isValid() const {
    return m_buffer != nullptr;
}
