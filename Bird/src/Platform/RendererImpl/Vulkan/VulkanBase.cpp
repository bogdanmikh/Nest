//
// Created by Bogdan on 29.05.2025.
//
#include "VulkanBase.hpp"

VkDevice *g_device = nullptr;
VkAllocationCallbacks *g_allocatorCb = nullptr;

void vkDestroy(VkCommandPool &obj) {
    if (obj != nullptr) {
        vkDestroyCommandPool(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkDescriptorPool &obj) {
    if (obj != nullptr) {
        vkDestroyDescriptorPool(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkDescriptorSetLayout &obj) {
    if (obj != nullptr) {
        vkDestroyDescriptorSetLayout(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkFence &obj) {
    if (obj != nullptr) {
        vkDestroyFence(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkFramebuffer &obj) {
    if (obj != nullptr) {
        vkDestroyFramebuffer(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkImage &obj) {
    if (obj != nullptr) {
        vkDestroyImage(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkImageView &obj) {
    if (obj != nullptr) {
        vkDestroyImageView(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkSampler &obj) {
    if (obj != nullptr) {
        vkDestroySampler(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkPipeline &obj) {
    if (obj != nullptr) {
        vkDestroyPipeline(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkPipelineCache &obj) {
    if (obj != nullptr) {
        vkDestroyPipelineCache(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkPipelineLayout &obj) {
    if (obj != nullptr) {
        vkDestroyPipelineLayout(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkRenderPass &obj) {
    if (obj != nullptr) {
        vkDestroyRenderPass(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkSemaphore &obj) {
    if (obj != nullptr) {
        vkDestroySemaphore(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkShaderModule &obj) {
    if (obj != nullptr) {
        vkDestroyShaderModule(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkSwapchainKHR &obj) {
    if (obj != nullptr) {
        vkDestroySwapchainKHR(*g_device, obj, g_allocatorCb);
        obj = nullptr;
    }
};