//
// Created by Bogdan on 07.07.2025.
//

#pragma once

#include "VulkanBase.hpp"

struct VulkanBufferDelegate {
    virtual ~VulkanBufferDelegate() = default;
    virtual VkResult allocateMemory(
        const VkMemoryRequirements *requirements,
        VkMemoryPropertyFlags propertyFlags,
        VkDeviceMemory *memory
    ) const = 0;
    virtual VkCommandBuffer getCommandBuffer() = 0;
    virtual void setMemoryBarrier(
        VkCommandBuffer commandBuffer,
        VkPipelineStageFlags srcStages,
        VkPipelineStageFlags dstStages
    ) = 0;
};

enum struct BufferType { Vertex, Index };

class VulkanBuffer {
public:
    void create(void *data, uint32_t size, BufferType type, bool isDynamic);
    void update(void *data, uint32_t size);
    void terminate();

    VulkanBufferDelegate *m_delegate;
    VkBuffer m_buffer;

private:
    uint32_t m_id;
    uint32_t m_size;
    bool m_isDynamic;
    VkDeviceMemory m_deviceMemory;
};
