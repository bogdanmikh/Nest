//
// Created by Bogdan on 14.05.2025.
//

#pragma once

#include "Bird/Config.hpp"
#include <vulkan/vulkan.h>

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