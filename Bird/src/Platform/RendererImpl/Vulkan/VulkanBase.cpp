//
// Created by Bogdan on 29.05.2025.
//
#include "VulkanBase.hpp"

VkDevice *device = nullptr;
VkAllocationCallbacks *allocatorCb = nullptr;

void vkDestroy(VkCommandPool &obj) {
    if (obj != nullptr) {
        vkDestroyCommandPool(*device, obj, allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkDescriptorPool &obj) {
    if (obj != nullptr) {
        vkDestroyDescriptorPool(*device, obj, allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkDescriptorSetLayout &obj) {
    if (obj != nullptr) {
        vkDestroyDescriptorSetLayout(*device, obj, allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkFence &obj) {
    if (obj != nullptr) {
        vkDestroyFence(*device, obj, allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkFramebuffer &obj) {
    if (obj != nullptr) {
        vkDestroyFramebuffer(*device, obj, allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkImage &obj) {
    if (obj != nullptr) {
        vkDestroyImage(*device, obj, allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkImageView &obj) {
    if (obj != nullptr) {
        vkDestroyImageView(*device, obj, allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkSampler &obj) {
    if (obj != nullptr) {
        vkDestroySampler(*device, obj, allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkPipeline &obj) {
    if (obj != nullptr) {
        vkDestroyPipeline(*device, obj, allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkPipelineCache &obj) {
    if (obj != nullptr) {
        vkDestroyPipelineCache(*device, obj, allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkPipelineLayout &obj) {
    if (obj != nullptr) {
        vkDestroyPipelineLayout(*device, obj, allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkRenderPass &obj) {
    if (obj != nullptr) {
        vkDestroyRenderPass(*device, obj, allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkSemaphore &obj) {
    if (obj != nullptr) {
        vkDestroySemaphore(*device, obj, allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkShaderModule &obj) {
    if (obj != nullptr) {
        vkDestroyShaderModule(*device, obj, allocatorCb);
        obj = nullptr;
    }
};
void vkDestroy(VkSwapchainKHR &obj) {
    if (obj != nullptr) {
        vkDestroySwapchainKHR(*device, obj, allocatorCb);
        obj = nullptr;
    }
};