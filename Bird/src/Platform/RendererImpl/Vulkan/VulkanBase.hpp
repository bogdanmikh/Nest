//
// Created by Bogdan on 14.05.2025.
//

#pragma once

#include "Bird/Config.hpp"

#include <unordered_map>
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

extern VkDevice *g_device;
extern VkAllocationCallbacks *g_allocatorCb;

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

template<typename Ty>
class StateCacheT {
public:
    void add(uint64_t key, Ty value) {
        invalidate(key);
        m_hashMap.insert(std::make_pair(key, value));
    }

    Ty find(uint64_t key) {
        typename HashMap::iterator it = m_hashMap.find(key);
        if (it != m_hashMap.end()) {
            return it->second;
        }

        return 0;
    }

    void invalidate(uint64_t key) {
        typename HashMap::iterator it = m_hashMap.find(key);
        if (it != m_hashMap.end()) {
            vkDestroy(it->second);
            m_hashMap.erase(it);
        }
    }

    void invalidate() {
        for (typename HashMap::iterator it = m_hashMap.begin(), itEnd = m_hashMap.end();
             it != itEnd;
             ++it) {
            vkDestroy(it->second);
        }

        m_hashMap.clear();
    }

    uint32_t getCount() const {
        return uint32_t(m_hashMap.size());
    }

private:
    typedef std::unordered_map<uint64_t, Ty> HashMap;
    HashMap m_hashMap;
};

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