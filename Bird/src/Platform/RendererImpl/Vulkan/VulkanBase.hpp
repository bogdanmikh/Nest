//
// Created by Bogdan on 14.05.2025.
//

#pragma once

#include "Bird/Config.hpp"

#include <vulkan/vulkan.h>

extern VkDevice *device;
extern VkAllocationCallbacks *allocatorCb;

void vkDestroy(VkCommandPool &obj);
void vkDestroy(VkDescriptorPool &obj);
void vkDestroy(VkDescriptorSetLayout &obj);
void vkDestroy(VkFence &obj);
void vkDestroy(VkFramebuffer &obj);
void vkDestroy(VkImage &obj);
void vkDestroy(VkImageView &obj);
void vkDestroy(VkSampler &obj);
void vkDestroy(VkPipeline &obj);
void vkDestroy(VkPipelineCache &obj);
void vkDestroy(VkPipelineLayout &obj);
void vkDestroy(VkRenderPass &obj);
void vkDestroy(VkSemaphore &obj);
void vkDestroy(VkShaderModule &obj);
void vkDestroy(VkSwapchainKHR &obj);

#if BIRD_LOG_ENABLED == 1
#    define VK_CHECK(result)                                                                       \
        if (result != VK_SUCCESS) {                                                                \
            LOG_ERROR("Vulkan error: {}", string_VkResult(result));                                \
            assert(false);                                                                         \
        }
#else
#    define VK_CHECK(result)                                                                       \
        if (result != VK_SUCCESS) {                                                                \
            assert(false);                                                                         \
        }
#endif