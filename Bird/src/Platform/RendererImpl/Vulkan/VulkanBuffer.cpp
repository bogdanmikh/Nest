//
// Created by Bogdan on 07.07.2025.
//

#include "VulkanBuffer.hpp"
#include "Foundation/Assert.hpp"

const VkAllocationCallbacks *allocatorCb = g_allocatorCb;
const VkDevice device = *g_device;

void VulkanBuffer::create(void *data, uint32_t size, BufferType type, bool isDynamic) {
    NEST_ASSERT(m_id == -1, "VERTEX BUFFER ALREADY CREATED");
    m_isDynamic = isDynamic;
    m_size = size;

    VkBufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.pNext = NULL;
    bufferCreateInfo.flags = 0;
    bufferCreateInfo.size = m_size;
    switch (type) {
        case BufferType::Vertex:
            bufferCreateInfo.usage = 0 | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        case BufferType::Index:
            bufferCreateInfo.usage = 0 | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    }
    bufferCreateInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.queueFamilyIndexCount = 0;
    bufferCreateInfo.pQueueFamilyIndices = NULL;

    VK_CHECK(vkCreateBuffer(device, &bufferCreateInfo, allocatorCb, &m_buffer));

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(device, m_buffer, &memoryRequirements);

    VK_CHECK(m_delegate->allocateMemory(
        &memoryRequirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &m_deviceMemory
    ));

    VK_CHECK(vkBindBufferMemory(device, m_buffer, m_deviceMemory, 0));

    if (!m_isDynamic) {
        update(data, size);
    }

}

void VulkanBuffer::update(void *data, uint32_t size) {
    vkMapMemory(device, m_deviceMemory, 0, size, 0, &data);
    vkUnmapMemory(device, m_deviceMemory);

//    m_delegate->setMemoryBarrier(
//        m_delegate->getCommandBuffer(),
//        VK_PIPELINE_STAGE_TRANSFER_BIT,
//        VK_PIPELINE_STAGE_TRANSFER_BIT
//    );
}

void VulkanBuffer::terminate() {
    vkFreeMemory(device, m_deviceMemory, allocatorCb);
    vkDestroyBuffer(device, m_buffer, allocatorCb);
}
