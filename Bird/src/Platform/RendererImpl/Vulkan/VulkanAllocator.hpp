//
// Created by Bogdan on 28.05.2025.
//

#pragma once

#include "VulkanBase.hpp"
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

/*

namespace Bird {

class VulkanMemoryAllocator {
public:
    VulkanMemoryAllocator() = default;
    ~VulkanMemoryAllocator() {
        cleanup();
    }

    bool init(VkInstance instance, VkDevice device, VkPhysicalDevice physicalDevice, uint32_t
apiVersion) { if (m_initialized) { return true;
        }

        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.vulkanApiVersion = apiVersion;
        allocatorInfo.physicalDevice = physicalDevice;
        allocatorInfo.device = device;
        allocatorInfo.instance = instance;

        if (vmaCreateAllocator(&allocatorInfo, &m_allocator) != VK_SUCCESS) {
            return false;
        }

        m_initialized = true;
        return true;
    }

    void cleanup() {
        if (m_allocator) {
            vmaDestroyAllocator(m_allocator);
            m_allocator = nullptr;
        }
        m_initialized = false;
    }

    VmaAllocator getAllocator() const {
        return m_allocator;
    }

private:
    VmaAllocator m_allocator = nullptr;
    bool m_initialized = false;
};

class VulkanAllocator {
public:
    static VmaAllocator getMemoryAllocator() {
        return allocator.getAllocator();
    }

    static bool initMemoryAllocator(VkInstance instance, VkDevice device, VkPhysicalDevice
physicalDevice, uint32_t apiVersion) { return get().init(instance, device, physicalDevice,
apiVersion);
    }

    static void cleanupMemoryAllocator() {
        get().cleanup();
    }
private:
    static VulkanMemoryAllocator& get() {
        return allocator;
    }
    static VulkanMemoryAllocator allocator;
};

} // namespace Bird

*/