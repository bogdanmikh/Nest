//
// Created by Admin on 14.03.2022.
//

#include "RendererVulkan.hpp"
#include "VulkanAllocator.hpp"

#include <Foundation/Assert.hpp>
#include <Foundation/Allocator.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/PlatformDetection.hpp>
#include <Foundation/Assert.hpp>
#include "Bird/PlatformData.hpp"
#include "Bird/Bird.hpp"

#include <sstream>
#include <set>

#ifdef PLATFORM_IOS
#    include "Platform/RendererImpl/Context/GlesContext.hpp"
#elif defined(PLATFORM_ANDROID)
#    include "Platform/RendererImpl/Context/AndroidContext.hpp"
#elif defined(PLATFORM_DESKTOP)
#    include "Platform/RendererImpl/Context/VulkanContext.hpp"
#    include <vulkan/vk_enum_string_helper.h>
#endif

#ifdef PLATFORM_MACOS
#    include <vulkan/vulkan_beta.h>
#endif

struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

Bird::Size getSize() {
    Bird::Size size;
    void *nativeWindow = Bird::PlatformData::get()->nativeWindowHandle;
#ifdef PLATFORM_DESKTOP
    GLFWwindow *glfWwindow = static_cast<GLFWwindow *>(nativeWindow);

    glfwGetWindowSize(glfWwindow, &size.width, &size.height);
#endif
    return size;
}

const char **getRequiredInstanceExtensions(uint32_t *extensionCount) {
    static std::vector<const char *> extensions;
#if defined(PLATFORM_WINDOWS)
    extensions = {VK_KHR_WIN32_SURFACE_EXTENSION_NAME};
#elif defined(PLATFORM_ANDROID)
    extensions = {VK_KHR_ANDROID_SURFACE_EXTENSION_NAME};
#elif defined(PLATFORM_LINUX)
    if (std::getenv("WAYLAND_DISPLAY")) {
        extensions = {VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME};
    } else {
        extensions = {VK_KHR_XLIB_SURFACE_EXTENSION_NAME};
    }
#elif defined(PLATFORM_MACOS)
    extensions = {"VK_EXT_metal_surface", VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME};
#endif
    extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
    *extensionCount = static_cast<uint32_t>(extensions.size());
    return extensions.data();
}

bool supported(
    const std::vector<const char *> &needExtensions, const std::vector<const char *> &layers
) {
    uint32_t extensionCount = 0;
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr));
    VkExtensionProperties extensionsProperties[extensionCount];
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionsProperties)
    );
#if BIRD_DEBUG_MODE
    std::ostringstream message;
    // functions that Vulkan supports
    message << "Device can support the following extensions:";
    for (int i = 0; i < extensionCount; i++) {
        VkExtensionProperties supportedExtension = extensionsProperties[i];
        message << "\n\t" << static_cast<std::string>(supportedExtension.extensionName);
    }
    LOG_INFO("{}", message.str().c_str());
#endif

    for (const auto &extension : needExtensions) {
        bool canSupport = false;
        for (int i = 0; i < extensionCount; i++) {
            VkExtensionProperties supportedExtension = extensionsProperties[i];
            if (strcmp(extension, supportedExtension.extensionName) == 0) {
                canSupport = true;
#if BIRD_DEBUG_MODE
                message << "\n\tExtension \"" << extension << "\" is supported";
#endif
                break;
            }
        }
        if (!canSupport) {
#if BIRD_DEBUG_MODE
            message << "\n\tExtension \"" << extension << "\" is not supported";
            LOG_INFO("{}", message.str().c_str());
#endif
            return false;
        }
    }
#if BIRD_DEBUG_MODE
    message.str("");
    LOG_INFO("{}", message.str().c_str());
#endif

    // check device can support layers
    uint32_t layerCount = 0;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));
    std::vector<VkLayerProperties> supportedLayers(layerCount);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, supportedLayers.data()));
#if BIRD_DEBUG_MODE
    message << "\n\tDevice can support the following layers:";
    for (const auto &supportedLayer : supportedLayers) {
        message << "\n\t" << static_cast<std::string>(supportedLayer.layerName);
    }
    LOG_INFO("{}", message.str().c_str());
    message.str("");
#endif
    for (const auto &layer : layers) {
        bool canSupport = false;
        for (const auto &supportedLayer : supportedLayers) {
            if (strcmp(layer, supportedLayer.layerName) == 0) {
                canSupport = true;
#if BIRD_DEBUG_MODE
                message << "\n\tLayer \"" << static_cast<std::string>(supportedLayer.layerName)
                        << "\" is supported";
#endif
                break;
            }
        }
        if (!canSupport) {
#if BIRD_DEBUG_MODE
            message << "\n\tLayer \"" << static_cast<std::string>(layer) << "\" is not supported";
            LOG_INFO("{}", message.str().c_str());
#endif
            return false;
        }
    }
#if BIRD_DEBUG_MODE
    LOG_INFO("{}", message.str().c_str());
#endif
    return true;
}

std::array<bool, 5> getDeviceProperties(const VkPhysicalDevice &device) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);
#if BIRD_DEBUG_MODE
    std::string deviceName = properties.deviceName;
    LOG_INFO("Device name: {}", deviceName.c_str());
#endif

    std::array<bool, 5> typeGPU{false};
    // 0 - Discrete GPU, 1 - Integrated GPU, 2 - Virtual GPU, 3 - CPU, 4 - Other
    switch (properties.deviceType) {
        case (VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU):
            typeGPU[0] = true;
        case (VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU):
            typeGPU[1] = true;
        case (VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU):
            typeGPU[2] = true;
        case (VK_PHYSICAL_DEVICE_TYPE_CPU):
            typeGPU[3] = true;
        default:
            typeGPU[4] = true;
    }
    return typeGPU;
}

/// DEBUG MESSENGER
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData
) {
    std::ostringstream message;
    message << "Validation layer: ";
    message << pCallbackData->pMessage;
    LOG_ERROR("{}", message.str().c_str());
    return VK_FALSE;
}

VkResult createDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger
) {

    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT
    )vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != NULL) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void destroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks *pAllocator
) {

    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT
    )vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (func != NULL) {
        func(instance, debugMessenger, pAllocator);
    }
}

void cleanupDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger) {
    if (debugMessenger != VK_NULL_HANDLE) {
        destroyDebugUtilsMessengerEXT(instance, debugMessenger, NULL);
        debugMessenger = VK_NULL_HANDLE;
    }
}

bool checkDeviceExtensionSupport(
    const VkPhysicalDevice &physicalDevice, uint32_t size, const char **requestedExtensions
) {
    //    Check if a given physical device can satisfy a list of requested device extensions.
    std::set<std::string> requiredExtensions(requestedExtensions, requestedExtensions + size);

    std::ostringstream message;
#if BIRD_DEBUG_MODE
    message << "Device can support extensions:";
#endif
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    VkExtensionProperties *extensions =
        static_cast<VkExtensionProperties *>(malloc(sizeof(VkExtensionProperties) * extensionCount)
        );
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions);
    for (int i = 0; i < extensionCount; ++i) {
        auto &extension = extensions[i];
#if BIRD_DEBUG_MODE
        message << "\n\t" << static_cast<const char *>(extension.extensionName);
#endif
        // remove this from the list of required extensions (set checks for equality automatically)
        requiredExtensions.erase(extension.extensionName);
    }
    free(extensions);
#if BIRD_DEBUG_MODE
    LOG_INFO("{}", message.str());
#endif
    // if the set is empty then all requirements have been satisfied
    return requiredExtensions.empty();
}

bool isSuitable(const VkPhysicalDevice &device, uint32_t size, const char **requestedExtensions) {
    std::ostringstream message;
    message << "Checking if device is suitable";
#if BIRD_DEBUG_MODE
    message << "\n\tWe are requesting device extensions:";
    for (int i = 0; i < size; i++) {
        const char *extension = requestedExtensions[i];
        message << "\n\t\"" << std::string(extension) << "\"";
    }
#endif

    bool extensionsSupported = checkDeviceExtensionSupport(device, size, requestedExtensions);
    if (extensionsSupported) {
#if BIRD_DEBUG_MODE
        message << "\n\tDevice can support the requested extensions!";
        LOG_INFO("{}", message.str());
#endif
        return true;
    } else {
#if BIRD_DEBUG_MODE
        message << "\n\tDevice can't support the requested extensions!";
        LOG_INFO("{}", message.str());
#endif
        return false;
    }
}

std::vector<const char *> logTransformBits(const VkSurfaceTransformFlagsKHR &bits) {
    std::vector<const char *> result;
    if (bits & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        result.emplace_back("identity");
    }
    if (bits & VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR) {
        result.emplace_back("90 degree rotation");
    }
    if (bits & VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR) {
        result.emplace_back("180 degree rotation");
    }
    if (bits & VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR) {
        result.emplace_back("270 degree rotation");
    }
    if (bits & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR) {
        result.emplace_back("horizontal mirror");
    }
    if (bits & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR) {
        result.emplace_back("horizontal mirror, then 90 degree rotation");
    }
    if (bits & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR) {
        result.emplace_back("horizontal mirror, then 180 degree rotation");
    }
    if (bits & VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR) {
        result.emplace_back("horizontal mirror, then 270 degree rotation");
    }
    if (bits & VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR) {
        result.emplace_back("inherited");
    }

    return result;
}

std::vector<const char *> logAlphaCompositeBits(const VkCompositeAlphaFlagsKHR &bits) {
    std::vector<const char *> result;
    if (bits & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) {
        result.emplace_back("opaque (alpha ignored)");
    }
    if (bits & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR) {
        result.emplace_back("pre multiplied (alpha expected to already be multiplied in image)");
    }
    if (bits & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR) {
        result.emplace_back("post multiplied (alpha will be applied during composition)");
    }
    if (bits & VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR) {
        result.emplace_back("inherited");
    }

    return result;
}

std::vector<const char *> logImageUsageBits(const VkImageUsageFlags &bits) {
    std::vector<const char *> result;
    if (bits & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
        result.push_back("transfer src: image can be used as the source of a transfer command.");
    }
    if (bits & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
        result.push_back("transfer dst: image can be used as the destination of a transfer command."
        );
    }
    if (bits & VK_IMAGE_USAGE_SAMPLED_BIT) {
        result.push_back(
            "sampled: image can be used to create a VkImageView suitable for occupying a \
VkDescriptorSet slot either of type VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE or \
VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, and be sampled by a shader."
        );
    }
    if (bits & VK_IMAGE_USAGE_STORAGE_BIT) {
        result.push_back(
            "storage: image can be used to create a VkImageView suitable for occupying a \
VkDescriptorSet slot of type VK_DESCRIPTOR_TYPE_STORAGE_IMAGE."
        );
    }
    if (bits & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
        result.push_back(
            "color attachment: image can be used to create a VkImageView suitable for use as \
a color or resolve attachment in a VkFramebuffer."
        );
    }
    if (bits & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) {
        result.push_back("depth/stencil attachment: image can be used to create a VkImageView \
suitable for use as a depth/stencil or depth/stencil resolve attachment in a VkFramebuffer.");
    }
    if (bits & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) {
        result.push_back(
            "transient attachment: implementations may support using memory allocations \
with the VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT to back an image with this usage. This \
bit can be set for any image that can be used to create a VkImageView suitable for use as \
a color, resolve, depth/stencil, or input attachment."
        );
    }
    if (bits & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) {
        result.push_back("input attachment: image can be used to create a VkImageView suitable for \
occupying VkDescriptorSet slot of type VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT; be read from \
a shader as an input attachment; and be used as an input attachment in a framebuffer.");
    }
    if (bits & VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT) {
        result.push_back("fragment density map: image can be used to create a VkImageView suitable \
for use as a fragment density map image.");
    }
    if (bits & VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR) {
        result.push_back(
            "fragment shading rate attachment: image can be used to create a VkImageView \
suitable for use as a fragment shading rate attachment or shading rate image"
        );
    }
    return result;
}

const char *logPresentMode(const VkPresentModeKHR &presentMode) {
    if (presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
        return "immediate: the presentation engine does not wait for a vertical blanking period \
to update the current image, meaning this mode may result in visible tearing. No internal \
queuing of presentation requests is needed, as the requests are applied immediately.";
    }
    if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
        return "mailbox: the presentation engine waits for the next vertical blanking period \
to update the current image. Tearing cannot be observed. An internal single-entry queue is \
used to hold pending presentation requests. If the queue is full when a new presentation \
request is received, the new request replaces the existing entry, and any images associated \
with the prior entry become available for re-use by the application. One request is removed \
from the queue and processed during each vertical blanking period in which the queue is non-empty.";
    }
    if (presentMode == VK_PRESENT_MODE_FIFO_KHR) {
        return "fifo: the presentation engine waits for the next vertical blanking \
period to update the current image. Tearing cannot be observed. An internal queue is used to \
hold pending presentation requests. New requests are appended to the end of the queue, and one \
request is removed from the beginning of the queue and processed during each vertical blanking \
period in which the queue is non-empty. This is the only value of presentMode that is required \
to be supported.";
    }
    if (presentMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR) {
        return "relaxed fifo: the presentation engine generally waits for the next vertical \
blanking period to update the current image. If a vertical blanking period has already passed \
since the last update of the current image then the presentation engine does not wait for \
another vertical blanking period for the update, meaning this mode may result in visible tearing \
in this case. This mode is useful for reducing visual stutter with an application that will \
mostly present a new image before the next vertical blanking period, but may occasionally be \
late, and present a new image just after the next vertical blanking period. An internal queue \
is used to hold pending presentation requests. New requests are appended to the end of the queue, \
and one request is removed from the beginning of the queue and processed during or after each \
vertical blanking period in which the queue is non-empty.";
    }
    if (presentMode == VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR) {
        return "shared demand refresh: the presentation engine and application have \
concurrent access to a single image, which is referred to as a shared presentable image. \
The presentation engine is only required to update the current image after a new presentation \
request is received. Therefore the application must make a presentation request whenever an \
update is required. However, the presentation engine may update the current image at any point, \
meaning this mode may result in visible tearing.";
    }
    if (presentMode == VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR) {
        return "shared continuous refresh: the presentation engine and application have \
concurrent access to a single image, which is referred to as a shared presentable image. The \
presentation engine periodically updates the current image on its regular refresh cycle. The \
application is only required to make one initial presentation request, after which the \
presentation engine must update the current image without any need for further presentation \
requests. The application can indicate the image contents have been updated by making a \
presentation request, but this does not guarantee the timing of when it will be updated. \
This mode may result in visible tearing if rendering to the image is not timed correctly.";
    }
    return "none/undefined";
}

SwapchainSupportDetails
querySwapchainSupport(const VkPhysicalDevice &device, const VkSurfaceKHR &surface) {
    SwapchainSupportDetails support;
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &support.capabilities));

    std::ostringstream message;
#if BIRD_DEBUG_MODE
    message << "Swapchain INFO:\n";
    message << "\tMinimum image count: " << support.capabilities.minImageCount << "\n";
    message << "\tMaximum image count: " << support.capabilities.maxImageCount << "\n";

    message << "\t\tWidth: " << support.capabilities.currentExtent.width << "\n";
    message << "\t\tHeight: " << support.capabilities.currentExtent.height << "\n";

    message << "\tMinimum supported extent: \n";
    message << "\t\tWidth: " << support.capabilities.minImageExtent.width << "\n";
    message << "\t\tHeight: " << support.capabilities.minImageExtent.height << "\n";

    message << "\tMaximum supported extent: \n";
    message << "\t\tWidth: " << support.capabilities.maxImageExtent.width << "\n";
    message << "\t\tHeight: " << support.capabilities.maxImageExtent.height << "\n";

    message << "\tMaximum image array layers: " << support.capabilities.maxImageArrayLayers << "\n";

    message << "\tSupported transforms:\n";
    std::vector<const char *> stringList =
        logTransformBits(support.capabilities.supportedTransforms);
    for (const auto &line : stringList) {
        message << "\t\t" << line << '\n';
    }

    message << "\tCurrent transform:\n";
    stringList = logTransformBits(support.capabilities.currentTransform);
    for (const auto &line : stringList) {
        message << "\t\t" << line << '\n';
    }

    message << "\tSupported alpha operations:\n";
    stringList = logAlphaCompositeBits(support.capabilities.supportedCompositeAlpha);
    for (const auto &line : stringList) {
        message << "\t\t" << line << "\n";
    }

    message << "\tsupported image usage:\n";
    stringList = logImageUsageBits(support.capabilities.supportedUsageFlags);
    for (const auto &line : stringList) {
        message << "\t\t" << line << "\n";
    }
#endif

    uint32_t count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, nullptr);
    support.formats.resize(count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &count, support.formats.data());

#if BIRD_DEBUG_MODE
    for (const auto &supportedFormat : support.formats) {
        message << "\tSupported pixel format: " << string_VkFormat(supportedFormat.format) << "\n";
        message << "\tSupported color space: " << string_VkColorSpaceKHR(supportedFormat.colorSpace)
                << "\n";
    }
#endif
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, nullptr);
    support.presentModes.resize(count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &count, support.presentModes.data());
#if BIRD_DEBUG_MODE
    for (const auto &presentMode : support.presentModes) {
        message << "\t" << logPresentMode(presentMode) << "\n";
    }
    std::string messageStr = std::string(message.str());
    messageStr.erase(messageStr.end() - 1);
    LOG_INFO("{}", messageStr.c_str());
#endif
    return support;
}

VkSurfaceFormatKHR chooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats) {
    for (const auto &format : formats) {
        // color format an 0-255, non linear
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM &&
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }
    return formats[0];
}

VkPresentModeKHR chooseSwapchainPresentMode(const std::vector<VkPresentModeKHR> &presentModes) {
    for (const auto &presentMode : presentModes) {
        // rendering mode
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return presentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapchainExtent(
    uint32_t width, uint32_t height, const VkSurfaceCapabilitiesKHR &capabilities
) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D extent = {width, height};

        extent.width = std::min(
            capabilities.maxImageExtent.width,
            std::max(capabilities.minImageExtent.width, extent.width)
        );

        extent.height = std::min(
            capabilities.maxImageExtent.height,
            std::max(capabilities.minImageExtent.height, extent.height)
        );

        return extent;
    }
}

void setBlendState(VkPipelineColorBlendStateCreateInfo &colorBlendState, uint64_t state) {
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.pNext = nullptr;
    colorBlendState.logicOpEnable = VK_FALSE;
    colorBlendState.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &colorBlendAttachment;
    colorBlendState.blendConstants[0] = 0.0f; // Optional
    colorBlendState.blendConstants[1] = 0.0f; // Optional
    colorBlendState.blendConstants[2] = 0.0f; // Optional
    colorBlendState.blendConstants[3] = 0.0f; // Optional
}

void setRasterizerState(
    VkPipelineRasterizationStateCreateInfo &rasterizationState,
    uint64_t state,
    bool wireframe = false
) {
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.pNext = NULL;
    rasterizationState.flags = 0;
    rasterizationState.depthClampEnable = state & BIRD_STATE_DEPTH_CLAMP;
    rasterizationState.rasterizerDiscardEnable = VK_FALSE;
    rasterizationState.polygonMode = wireframe ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
    if (state & BIRD_STATE_CULL_FACE) {
        rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
    } else {
        rasterizationState.cullMode = VK_CULL_MODE_NONE;
    }
    rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizationState.depthBiasEnable = VK_FALSE;
    rasterizationState.depthBiasConstantFactor = 0.0f;
    rasterizationState.depthBiasClamp = 0.0f;
    rasterizationState.depthBiasSlopeFactor = 0.0f;
    rasterizationState.lineWidth = 1.0f;
}

namespace Bird {

RendererVulkan::RendererVulkan()
    : m_uselessVao(0)
    , m_frameBuffers()
    , m_shaders()
    , m_indexBuffers()
    , m_vertexLayouts()
    , m_vertexBuffers()
    , m_textures() {
    g_allocatorCb = m_allocatorCb;
#ifdef PLATFORM_DESKTOP
    NEST_ASSERT(glfwVulkanSupported(), "GLFW NOT SUPPORT VULKAN!")
    context = F_NEW(Foundation::getAllocator(), VulkanContext);
#endif
    context->create();
    createInstance();
#if BIRD_DEBUG_MODE
    setupDebugMessenger();
#endif
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    g_device = &m_device;

    createSwapchain(getSize(), nullptr);
    createSwapchainRenderPass();
    createSwapchainFramebuffer();
    createFence();
    createSemaphores();
    createCommandPool();
}

void RendererVulkan::createInstance() {
    apiVersion = VK_API_VERSION_1_2;
    BIRD_LOG("Make instance...");
    uint32_t version;
    VK_CHECK(vkEnumerateInstanceVersion(&version));
    BIRD_LOG(
        "System can support Vulkan version: {}.{}.{}.{}",
        VK_API_VERSION_VARIANT(version),
        VK_API_VERSION_MAJOR(version),
        VK_API_VERSION_MINOR(version),
        VK_API_VERSION_PATCH(version)
    );

    // patch = 0
    version &= ~(0xFFFU);

    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "PandaEditor";
    appInfo.pEngineName = "Panda";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    //    appInfo.apiVersion = version;
    appInfo.apiVersion = apiVersion;
    appInfo.pNext = nullptr;

    uint32_t extensionCount = 0;
    const char **deviceExtensions = getRequiredInstanceExtensions(&extensionCount);
    std::vector<const char *> extensions(deviceExtensions, deviceExtensions + extensionCount);
#if BIRD_DEBUG_MODE
    // add extension utils for debug
    extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    std::ostringstream message;
    message << "Extension to be requested:\n";

    for (int i = 0; i < extensions.size() - 1; ++i) {
        message << "\t" << extensions[i] << "\n";
    }
    message << "\t" << extensions.back();

    LOG_INFO("{}", message.str().c_str());
#endif

    std::vector<const char *> layers;
#if BIRD_DEBUG_MODE
    // Проверяет вызовы Vulkan API на соответствие спецификации, выявляет ошибки, предупреждения и
    // потенциальные проблемы производительности.
    layers.push_back("VK_LAYER_KHRONOS_validation");

    // Логирует все вызовы Vulkan API и их параметры в консоль или файл. Полезен для глубокой
    // отладки.
    //    layers.push_back("VK_LAYER_LUNARG_api_dump");

    // Выводит в реальном времени статистику по использованию Vulkan (FPS, количество вызовов и т.
    // д.).
    //    layers.push_back("VK_LAYER_LUNARG_monitor");

    // Анализирует код на предмет best practices и даёт рекомендации по оптимизации.
//    layers.push_back("VK_LAYER_LUNARG_assistant_layer");
#endif
    NEST_ASSERT(
        supported(extensions, layers),
        "This device does not support the necessary extensions or layers for Vulkan"
    );

    VkInstanceCreateInfo instanceCreateInfo;
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    instanceCreateInfo.pApplicationInfo = &appInfo;
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
    instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    instanceCreateInfo.ppEnabledLayerNames = layers.data();
    instanceCreateInfo.pNext = nullptr;
    VK_CHECK(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));
}

void RendererVulkan::setupDebugMessenger() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;

    VK_CHECK(createDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger));
}

void RendererVulkan::createSurface() {
#ifdef PLATFORM_DESKTOP
    VkSurfaceKHR surface;
    auto *window = static_cast<GLFWwindow *>(PlatformData::get()->nativeWindowHandle);
    VK_CHECK(glfwCreateWindowSurface(m_instance, window, nullptr, &surface));

    m_surface = surface;

    BIRD_LOG("Successfully abstracted GLFW surface for Vulkan");
#endif
}

void RendererVulkan::pickPhysicalDevice() {
    BIRD_LOG("Choosing physical device...");
    uint32_t physicalDeviceCount = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, nullptr));

    std::vector<VkPhysicalDevice> physicalDevices;
    physicalDevices.resize(physicalDeviceCount);

    VK_CHECK(vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, &physicalDevices[0]));
#if BIRD_DEBUG_MODE
    std::ostringstream message;
    message << "There are: " << physicalDevices.size()
            << " physical devices available on the system";
    LOG_INFO("{}", message.str().c_str());
#endif
    std::vector<std::array<bool, 5>> devicesTypes;
    for (const auto &device : physicalDevices) {
        devicesTypes.emplace_back(getDeviceProperties(device));
    }

    std::vector<const char *> extensions;
    extensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
#ifdef PLATFORM_MACOS
    extensions.emplace_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
#endif
    for (int numType = 0; numType < 5; ++numType) {
        for (int numDevice = 0; numDevice < physicalDeviceCount; ++numDevice) {
            if (devicesTypes[numDevice][numType] &&
                isSuitable(physicalDevices[numDevice], extensions.size(), extensions.data())) {
                m_physicalDevice = physicalDevices[numDevice];
            }
        }
    }

    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_memoryProperties);
}

void RendererVulkan::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies();
    NEST_ASSERT(indices.isComplete(), "Indices not complete");
    float queuePriority = 1.0f;
    std::set<uint32_t> uniqueQueueFamilies;
    uniqueQueueFamilies.emplace(indices.graphicsFamily);
    uniqueQueueFamilies.emplace(indices.presentFamily);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    for (const uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.emplace_back(queueCreateInfo);
    }

    std::vector<const char *> enabledLayers;
#if BIRD_DEBUG_MODE
    enabledLayers.emplace_back("VK_LAYER_KHRONOS_validation");
#endif

    std::vector<const char *> extensions;
    extensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
#ifdef PLATFORM_MACOS
    extensions.emplace_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
#endif

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.enabledLayerCount = enabledLayers.size();
    deviceCreateInfo.ppEnabledLayerNames = enabledLayers.data();
    deviceCreateInfo.enabledExtensionCount = extensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = extensions.data();
    deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    VK_CHECK(vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device));

    vkGetDeviceQueue(m_device, indices.graphicsFamily, 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, indices.presentFamily, 0, &m_presentQueue);
}

void RendererVulkan::setupAllocator() {
    //    VulkanAllocator::initMemoryAllocator(m_instance, m_device, m_physicalDevice, apiVersion);
}

void RendererVulkan::createSwapchain(Size size, VkSwapchainKHR *oldSwapchain) {
    //    m_depthStencilFormat = VK_FORMAT_D24_UNORM_S8_UINT;
    m_depthStencilFormat = VK_FORMAT_D32_SFLOAT_S8_UINT;
    SwapchainSupportDetails support(std::move(querySwapchainSupport(m_physicalDevice, m_surface)));

    VkSurfaceFormatKHR format = chooseSwapchainSurfaceFormat(support.formats);

    VkPresentModeKHR presentMode = chooseSwapchainPresentMode(support.presentModes);

    VkExtent2D extent = chooseSwapchainExtent(size.width, size.height, support.capabilities);

    m_swapchainFormat = format.format;
    m_swapchainExtent = extent;

    uint32_t imageCount;
    if (support.capabilities.maxImageCount == 0) {
        imageCount = std::min(int(support.capabilities.minImageCount + 1), 3);
    } else {
        imageCount = std::min(
            std::min((int)support.capabilities.minImageCount + 1, 3),
            (int)support.capabilities.maxImageCount
        );
    }

    // Базовая инициализация структуры (обязательно для всех Vulkan-структур)
    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
    // Указываем тип структуры для корректной работы Vulkan API
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    // Поверхность (окно), с которой будет связан свопчейн
    swapchainCreateInfo.surface = m_surface;

    // Минимальное количество изображений в цепочке подкачки
    swapchainCreateInfo.minImageCount = imageCount;
    // Формат пикселей (например, VK_FORMAT_B8G8R8A8_UNORM)
    swapchainCreateInfo.imageFormat = format.format;
    // Цветовое пространство (например, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
    swapchainCreateInfo.imageColorSpace = format.colorSpace;
    // Разрешение изображений в свопчейне (ширина/высота в пикселях)
    swapchainCreateInfo.imageExtent = extent;

    // Как обрабатывать альфа-канал (здесь - игнорировать, делая окно непрозрачным)
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    // Оптимизация: не рендерить части изображения, скрытые другими окнами
    swapchainCreateInfo.clipped = VK_TRUE;

    // Определяет, как будут использоваться изображения свопчейна:
    // VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT - для рендеринга напрямую
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // Количество слоев в изображении (1 для обычных 2D-приложений)
    swapchainCreateInfo.imageArrayLayers = 1;

    // Получаем индексы семейств очередей
    QueueFamilyIndices indices = findQueueFamilies();
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily, indices.presentFamily};

    // Настройка совместного доступа к изображениям:
    if (indices.graphicsFamily != indices.presentFamily) {
        // CONCURRENT - если разные очереди для рендеринга и отображения
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        // Количество семейств очередей
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        // Указатель на массив семейств очередей
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        // EXCLUSIVE - если одна очередь и для рендеринга, и для отображения (оптимальнее)
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    // Трансформация изображения (например, поворот на 90° для мобильных устройств)
    swapchainCreateInfo.preTransform = support.capabilities.currentTransform;

    // Режим презентации (например, VK_PRESENT_MODE_MAILBOX_KHR для тройной буферизации)
    swapchainCreateInfo.presentMode = presentMode;

    // Обработка пересоздания свопчейна (например, при изменении размера окна)
    if (oldSwapchain != nullptr) {
        // Передаем старый свопчейн для корректного пересоздания
        swapchainCreateInfo.oldSwapchain = *oldSwapchain;
    } else {
        // Первое создание свопчейна
        swapchainCreateInfo.oldSwapchain = VkSwapchainKHR(nullptr);
    }
    VK_CHECK(vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, m_allocatorCb, &m_swapchain));
    VK_CHECK(vkGetSwapchainImagesKHR(m_device, m_swapchain, &m_numSwapchainImages, nullptr));
    VkImage images[m_numSwapchainImages];
    VK_CHECK(vkGetSwapchainImagesKHR(m_device, m_swapchain, &m_numSwapchainImages, &images[0]));

    VkImageCreateInfo imageCreateInfo;
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = NULL;
    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = m_depthStencilFormat;
    imageCreateInfo.extent.width = m_swapchainExtent.width;
    imageCreateInfo.extent.height = m_swapchainExtent.height;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.usage =
        0 | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = 0;  // m_sci.queueFamilyIndexCount;
    imageCreateInfo.pQueueFamilyIndices = NULL; // m_sci.pQueueFamilyIndices;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VK_CHECK(vkCreateImage(m_device, &imageCreateInfo, m_allocatorCb, &m_depthStencilImage));

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(m_device, m_depthStencilImage, &memoryRequirements);

    VK_CHECK(allocateMemory(
        &memoryRequirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &m_depthStencilMemory
    ));

    VK_CHECK(vkBindImageMemory(m_device, m_depthStencilImage, m_depthStencilMemory, 0));

    VkImageViewCreateInfo imageViewCreateInfo;
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.pNext = NULL;
    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.image = m_depthStencilImage;
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format = m_depthStencilFormat;
    imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.subresourceRange.aspectMask =
        0 | VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;
    VK_CHECK(
        vkCreateImageView(m_device, &imageViewCreateInfo, m_allocatorCb, &m_depthStencilImageView)
    );

    for (int i = 0; i < m_numSwapchainImages; ++i) {
        VkImageViewCreateInfo imageViewInfo{};
        imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewInfo.image = images[i];
        imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        // We can change the order of the colors
        imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewInfo.subresourceRange.baseMipLevel = 0;
        imageViewInfo.subresourceRange.levelCount = 1;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.layerCount = 1;
        imageViewInfo.format = format.format;

        m_swapchainFrames[i].image = images[i];
        VkImageView *imageView = &m_swapchainFrames[i].imageView;
        VK_CHECK(vkCreateImageView(
            m_device, &imageViewInfo, m_allocatorCb, &m_swapchainFrames[i].imageView
        ));
    }
}

int32_t RendererVulkan::selectMemoryType(
    uint32_t memoryTypeBits, uint32_t propertyFlags, int32_t startIndex
) const {
    for (int32_t i = startIndex, num = m_memoryProperties.memoryTypeCount; i < num; ++i) {
        const VkMemoryType &memType = m_memoryProperties.memoryTypes[i];
        if (((1 << i) & memoryTypeBits) != 0 &&
            (memType.propertyFlags & propertyFlags) == propertyFlags) {
            return i;
        }
    }

    BIRD_LOG("Failed to find memory that supports flags");
    return -1;
}

VkResult RendererVulkan::allocateMemory(
    const VkMemoryRequirements *requirements,
    VkMemoryPropertyFlags propertyFlags,
    VkDeviceMemory *memory
) const {
    VkMemoryAllocateInfo ma;
    ma.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    ma.pNext = NULL;
    ma.allocationSize = requirements->size;

    VkResult result = VK_ERROR_UNKNOWN;
    int32_t searchIndex = -1;
    do {
        searchIndex++;
        searchIndex = selectMemoryType(requirements->memoryTypeBits, propertyFlags, searchIndex);
        if (searchIndex >= 0) {
            ma.memoryTypeIndex = searchIndex;
            result = vkAllocateMemory(m_device, &ma, m_allocatorCb, memory);
        }
    } while (result != VK_SUCCESS && searchIndex >= 0);

    return result;
}

RendererVulkan::~RendererVulkan() {
    VK_CHECK(vkQueueWaitIdle(m_graphicsQueue));
    VK_CHECK(vkDeviceWaitIdle(m_device));

    for (uint32_t i = 0; i < MAX_FRAME_BUFFERS; ++i) {
        m_frameBuffers[i].terminate();
    }

    for (uint32_t i = 0; i < MAX_INDEX_BUFFERS; ++i) {
        m_indexBuffers[i].terminate();
    }

    for (uint32_t i = 0; i < MAX_VERTEX_BUFFERS; ++i) {
        m_vertexBuffers[i].terminate();
    }

    for (uint32_t i = 0; i < MAX_SHADERS; ++i) {
        m_shaders[i].terminate();
    }

    for (uint32_t i = 0; i < MAX_TEXTURES; ++i) {
        m_textures[i].terminate();
    }

    m_pipelineStateCache.invalidate();
    m_descriptorSetLayoutCache.invalidate();
    m_renderPassCache.invalidate();
    m_samplerCache.invalidate();
    for (int i = 0; i < m_numSwapchainImages; ++i) {
        auto &swapchainFrame = m_swapchainFrames[i];
        vkFreeCommandBuffers(m_device, m_commandPool, 1, &swapchainFrame.commandBuffer);
    }

    for (uint32_t i = 0; i < m_numSwapchainImages; ++i) {
        vkDestroy(m_swapchainFrames[i].imageAvailable);
        vkDestroy(m_swapchainFrames[i].renderFinished);
    }
    releaseSwapchainFramebuffer();
    cleanupSwapchain();
    vkDestroySurfaceKHR(m_instance, m_surface, m_allocatorCb);
    vkDestroy(m_renderPass);
    //    VulkanAllocator::cleanupMemoryAllocator();
    vkDestroyDevice(m_device, nullptr);
#if BIRD_DEBUG_MODE
    cleanupDebugMessenger(m_instance, m_debugMessenger);
#endif
    vkDestroyInstance(m_instance, nullptr);
    F_DELETE(Foundation::getAllocator(), context);
}

RendererVulkan::QueueFamilyIndices RendererVulkan::findQueueFamilies() {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
        m_physicalDevice, &queueFamilyCount, queueFamilies.data()
    );

    std::ostringstream message;
#if BIRD_DEBUG_MODE
    message << "System can support " << queueFamilies.size() << " queue families";
#endif
    for (int i = 0; i < queueFamilyCount; i++) {
        VkQueueFamilyProperties &queueFamily = queueFamilies[i];
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
#if BIRD_DEBUG_MODE
            message << "\n\tQueue Family " << i << " is suitable for graphics";
#endif
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
#if BIRD_DEBUG_MODE
            message << "\n\tQueue Family " << i << " is suitable for presenting";
#endif
        }
        if (indices.isComplete()) {
            break;
        }
    }
#if BIRD_DEBUG_MODE
    LOG_INFO("{}", message.str());
#endif
    return indices;
}

void RendererVulkan::cleanupSwapchain() {
    for (auto &frame : m_swapchainFrames) {
        vkDestroy(frame.imageView);
        vkDestroy(frame.framebuffer);
    }
    vkDestroy(m_swapchain);
}

void RendererVulkan::createSwapchainFramebuffer() {
    for (uint32_t i = 0; i < m_numSwapchainImages; ++i) {
        SwapchainFrame &swapchainFrame = m_swapchainFrames[i];
        VkImageView attachments[] = {swapchainFrame.imageView, m_depthStencilImageView};

        VkFramebufferCreateInfo framebufferCreateInfo;
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.pNext = NULL;
        framebufferCreateInfo.flags = 0;
        framebufferCreateInfo.renderPass = m_renderPass;
        framebufferCreateInfo.attachmentCount = 2;
        framebufferCreateInfo.pAttachments = attachments;
        framebufferCreateInfo.width = m_swapchainExtent.width;
        framebufferCreateInfo.height = m_swapchainExtent.height;
        framebufferCreateInfo.layers = 1;

        VK_CHECK(vkCreateFramebuffer(
            m_device, &framebufferCreateInfo, m_allocatorCb, &swapchainFrame.framebuffer
        ));
    }
}

void RendererVulkan::createCommandPool() {
    QueueFamilyIndices indices = findQueueFamilies();

    VkCommandPoolCreateInfo commandPoolCreateInfo;
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.pNext = NULL;
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolCreateInfo.queueFamilyIndex = indices.graphicsFamily;

    VK_CHECK(vkCreateCommandPool(m_device, &commandPoolCreateInfo, m_allocatorCb, &m_commandPool));
    VkCommandBufferAllocateInfo commandBufferAllocateInfo;
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.pNext = NULL;
    commandBufferAllocateInfo.commandPool = m_commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = 1;

    for (int i = 0; i < m_numSwapchainImages; ++i) {
        VkResult result = vkAllocateCommandBuffers(
            m_device, &commandBufferAllocateInfo, &m_swapchainFrames[i].commandBuffer
        );
        if (result != VK_SUCCESS) {
            vkDestroy(m_commandPool);
            VK_CHECK(result);
        }
    }

    initSwapchainImageLayout();

    VK_CHECK(vkResetCommandPool(m_device, m_commandPool, 0));
}

void RendererVulkan::createSemaphores() {
    VkSemaphoreCreateInfo semaphoreCreateInfo;
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreCreateInfo.pNext = NULL;
    semaphoreCreateInfo.flags = 0;

    for (uint32_t i = 0; i < m_numSwapchainImages; ++i) {
        VK_CHECK(vkCreateSemaphore(
            m_device, &semaphoreCreateInfo, nullptr, &m_swapchainFrames[i].imageAvailable
        ));
    }
}

void RendererVulkan::createFence() {
    VkFenceCreateInfo fenceCreateInfo;
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.pNext = NULL;
    fenceCreateInfo.flags = 0;
    VK_CHECK(vkCreateFence(m_device, &fenceCreateInfo, m_allocatorCb, &m_fence));
}

void RendererVulkan::releaseSwapchainFramebuffer() {
    for (uint32_t i = 0; i < m_numSwapchainImages; ++i) {
        vkDestroy(m_swapchainFrames[i].imageView);
    }
}

void RendererVulkan::createSwapchainRenderPass() {
    VkAttachmentDescription attachmentDescription[2];
    attachmentDescription[0].flags = 0;
    attachmentDescription[0].format = m_swapchainFormat;
    attachmentDescription[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachmentDescription[0].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    attachmentDescription[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDescription[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDescription[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDescription[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachmentDescription[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachmentDescription[1].flags = 0;
    attachmentDescription[1].format = m_depthStencilFormat;
    attachmentDescription[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attachmentDescription[1].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    attachmentDescription[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDescription[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    attachmentDescription[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDescription[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    attachmentDescription[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentReference[1];
    colorAttachmentReference[0].attachment = 0;
    colorAttachmentReference[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference resolveAttachmentReference[1];
    resolveAttachmentReference[0].attachment = VK_ATTACHMENT_UNUSED;
    resolveAttachmentReference[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentReference[1];
    depthAttachmentReference[0].attachment = 1;
    depthAttachmentReference[0].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription[1];
    subpassDescription[0].flags = 0;
    subpassDescription[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription[0].inputAttachmentCount = 0;
    subpassDescription[0].pInputAttachments = NULL;
    subpassDescription[0].colorAttachmentCount = 1;
    subpassDescription[0].pColorAttachments = colorAttachmentReference;
    subpassDescription[0].pResolveAttachments = resolveAttachmentReference;
    subpassDescription[0].pDepthStencilAttachment = depthAttachmentReference;
    subpassDescription[0].preserveAttachmentCount = 0;
    subpassDescription[0].pPreserveAttachments = NULL;

    VkRenderPassCreateInfo renderPassCreateInfo;
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext = NULL;
    renderPassCreateInfo.flags = 0;
    renderPassCreateInfo.attachmentCount = 2;
    renderPassCreateInfo.pAttachments = attachmentDescription;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = subpassDescription;
    renderPassCreateInfo.dependencyCount = 0;
    renderPassCreateInfo.pDependencies = NULL;

    VK_CHECK(vkCreateRenderPass(m_device, &renderPassCreateInfo, m_allocatorCb, &m_renderPass));
}

void RendererVulkan::releaseSwapchainRenderPass() {
    vkDestroy(m_renderPass);
}

void RendererVulkan::initSwapchainImageLayout() {
    // ?
    //        VkCommandBuffer commandBuffer = beginNewCommand();
    //
    //        setImageMemoryBarrier(
    //            commandBuffer
    //            , m_backBufferDepthStencilImage
    //            , VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
    //            , VK_IMAGE_LAYOUT_UNDEFINED
    //            , VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    //            , 1
    //            , 1
    //        );
    //
    //        submitCommandAndWait(commandBuffer);
    m_imageIndex = 0;
}

VkPipelineMultisampleStateCreateInfo RendererVulkan::getMultisampleState(uint32_t stateFlags) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(m_physicalDevice, &properties);

    bool supports8xMSAA = properties.limits.framebufferColorSampleCounts & VK_SAMPLE_COUNT_8_BIT;
    bool supports4xMSAA = properties.limits.framebufferColorSampleCounts & VK_SAMPLE_COUNT_4_BIT;

    VkPipelineMultisampleStateCreateInfo multisampleState = {};
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.pNext = nullptr;
    multisampleState.flags = 0;
    multisampleState.pSampleMask = nullptr;
    multisampleState.alphaToOneEnable = VK_FALSE;
    // Настройка MSAA (мультисэмплинга)
    if (stateFlags & BIRD_STATE_MSAA_8X) {
        if (supports8xMSAA) {
            multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_8_BIT;
        } else {
            multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        }
    } else if (stateFlags & BIRD_STATE_MSAA_4X) {
        if (supports4xMSAA) {
            multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_4_BIT;
        } else {
            multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        }
    } else if (stateFlags & BIRD_STATE_MSAA_2X) {
        multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_2_BIT;
    } else {
        multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    }

    // Включение Sample Shading (если требуется высокое качество)
    multisampleState.sampleShadingEnable =
        (stateFlags & BIRD_STATE_SAMPLE_SHADING) ? VK_TRUE : VK_FALSE;
    multisampleState.minSampleShading = multisampleState.sampleShadingEnable ? 1.0f : 0.0f;

    // Включение Alpha-to-Coverage (для прозрачных объектов)
    multisampleState.alphaToCoverageEnable =
        (stateFlags & BIRD_STATE_ALPHA_TO_COVERAGE) ? VK_TRUE : VK_FALSE;

    return multisampleState;
}

uint32_t getPipelineHashkey(
    uint64_t state, Bird::ProgramHandle program, const Bird::VertexBufferLayoutData &layoutData
) {
    return program.id + state % 100 + layoutData.m_stride % 100;
}

VkPipeline RendererVulkan::getPipeline(
    uint64_t state, Bird::ProgramHandle program, const Bird::VertexBufferLayoutData &layoutData
) {
    VulkanShader &shader = m_shaders[program.id];

    uint32_t hashKey = getPipelineHashkey(state, program, layoutData);
    VkPipeline pipeline = m_pipelineStateCache.find(hashKey);

    if (pipeline != VK_NULL_HANDLE) {
        return pipeline;
    }

    // Blending
    VkPipelineColorBlendStateCreateInfo colorBlendState;
    setBlendState(colorBlendState, state);

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState;
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyState.pNext = NULL;
    inputAssemblyState.flags = 0;
    inputAssemblyState.primitiveRestartEnable = VK_FALSE;

    VkPipelineRasterizationStateCreateInfo rasterizationState;
    setRasterizerState(rasterizationState, state, false);

    VkPipelineDepthStencilStateCreateInfo depthStencilState;
    setDepthStencilState(depthStencilState, state);

    VkVertexInputAttributeDescription attributeDescriptions[MAX_VERTEX_LAYOUT_ELEMENTS];
    VkVertexInputBindingDescription bindingDescription[MAX_VERTEX_LAYOUT_ELEMENTS];

    VkPipelineVertexInputStateCreateInfo vertexInputState;
    vertexInputState.pVertexBindingDescriptions = bindingDescription;
    vertexInputState.pVertexAttributeDescriptions = attributeDescriptions;
    setInputLayout(vertexInputState, program, layoutData);

    static std::array<VkDynamicState, 2> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState;
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = NULL;
    dynamicState.flags = 0;
    dynamicState.dynamicStateCount = dynamicStates.size();
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineShaderStageCreateInfo shaderStages[2];
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].pNext = NULL;
    shaderStages[0].flags = 0;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = shader.m_vertex;
    shaderStages[0].pName = "main";
    shaderStages[0].pSpecializationInfo = NULL;

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].pNext = NULL;
    shaderStages[1].flags = 0;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = shader.m_fragment;
    shaderStages[1].pName = "main";
    shaderStages[1].pSpecializationInfo = NULL;

    VkPipelineViewportStateCreateInfo viewportState;
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = NULL;
    viewportState.flags = 0;
    viewportState.viewportCount = 1;
    viewportState.pViewports = NULL;
    viewportState.scissorCount = 1;
    viewportState.pScissors = NULL;

    VkPipelineMultisampleStateCreateInfo multisampleState = getMultisampleState(state);

    VkGraphicsPipelineCreateInfo graphicsPipeline;
    graphicsPipeline.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipeline.pNext = NULL;
    graphicsPipeline.flags = 0;
    graphicsPipeline.stageCount = 2;
    graphicsPipeline.pStages = shaderStages;
    graphicsPipeline.pVertexInputState = &vertexInputState;
    graphicsPipeline.pInputAssemblyState = &inputAssemblyState;
    graphicsPipeline.pTessellationState = NULL;
    graphicsPipeline.pViewportState = &viewportState;
    graphicsPipeline.pRasterizationState = &rasterizationState;
    graphicsPipeline.pMultisampleState = &multisampleState;
    graphicsPipeline.pDepthStencilState = &depthStencilState;
    graphicsPipeline.pColorBlendState = &colorBlendState;
    graphicsPipeline.pDynamicState = &dynamicState;
    graphicsPipeline.layout = shader.m_pipelineLayout;
    graphicsPipeline.renderPass = m_frameBufferHandle.isValid()
                                      ? m_frameBuffers[m_frameBufferHandle.id].m_renderPass
                                      : m_renderPass;
    graphicsPipeline.subpass = 0;
    graphicsPipeline.basePipelineHandle = VK_NULL_HANDLE;
    graphicsPipeline.basePipelineIndex = 0;

    // TODO: Cache pipelines
    VK_CHECK(
        vkCreateGraphicsPipelines(m_device, nullptr, 1, &graphicsPipeline, m_allocatorCb, &pipeline)
    );

    m_pipelineStateCache.add(program.id, pipeline);

    return pipeline;
}

uint32_t getRenderPassHashkey(uint32_t num, const FrameBufferAttachment *attachments) {
    uint32_t hash = 0;
    for (int i = 0; i < num; ++i) {
        hash += attachments[i].handle.id;
    }
    return hash;
}

VkRenderPass
RendererVulkan::getRenderPass(uint32_t num, const Bird::FrameBufferAttachment *attachments) {
    VkRenderPass renderPass = VK_NULL_HANDLE;
    uint32_t hashKey = getRenderPassHashkey(num, attachments);
    renderPass = (VkRenderPass)m_renderPassCache.find(hashKey);
    if (renderPass != VK_NULL_HANDLE) {
        return renderPass;
    }
    VkAttachmentDescription attachmentDescription[MAX_FRAMEBUFFER_ATTACHMENTS];
    VkAttachmentReference colorAttachmentDescription[MAX_FRAMEBUFFER_ATTACHMENTS];
    VkAttachmentReference resolveAttachmentDescription[MAX_FRAMEBUFFER_ATTACHMENTS];
    VkAttachmentReference depthAr;
    uint32_t numColorAr = 0;

    depthAr.attachment = VK_ATTACHMENT_UNUSED;
    depthAr.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    for (uint8_t i = 0; i < num; ++i) {
        VulkanTexture &texture = m_textures[attachments[i].handle.id];
        attachmentDescription[i].flags = 0;
        attachmentDescription[i].format = texture.m_format;
        attachmentDescription[i].samples = VK_SAMPLE_COUNT_1_BIT;

        if (texture.m_aspectMask & VK_IMAGE_ASPECT_COLOR_BIT) {
            attachmentDescription[i].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            attachmentDescription[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescription[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachmentDescription[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachmentDescription[i].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            attachmentDescription[i].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            colorAttachmentDescription[numColorAr].layout =
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            colorAttachmentDescription[numColorAr].attachment = i;
            numColorAr++;
        } else if (texture.m_aspectMask & VK_IMAGE_ASPECT_DEPTH_BIT) {
            attachmentDescription[i].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            attachmentDescription[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescription[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
            attachmentDescription[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachmentDescription[i].initialLayout =
                VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            attachmentDescription[i].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            depthAr.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            depthAr.attachment = i;
        }
        resolveAttachmentDescription[i].attachment = VK_ATTACHMENT_UNUSED;
        resolveAttachmentDescription[i].layout = attachmentDescription[i].initialLayout;
    }

    VkSubpassDescription subpassDescription[1];
    subpassDescription[0].flags = 0;
    subpassDescription[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription[0].inputAttachmentCount = 0;
    subpassDescription[0].pInputAttachments = NULL;
    subpassDescription[0].colorAttachmentCount = numColorAr;
    subpassDescription[0].pColorAttachments = colorAttachmentDescription;
    subpassDescription[0].pResolveAttachments = resolveAttachmentDescription;
    subpassDescription[0].pDepthStencilAttachment = &depthAr;
    subpassDescription[0].preserveAttachmentCount = 0;
    subpassDescription[0].pPreserveAttachments = NULL;

    VkRenderPassCreateInfo renderPassCreateInfo;
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.pNext = NULL;
    renderPassCreateInfo.flags = 0;
    renderPassCreateInfo.attachmentCount = num;
    renderPassCreateInfo.pAttachments = attachmentDescription;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = subpassDescription;
    renderPassCreateInfo.dependencyCount = 0;
    renderPassCreateInfo.pDependencies = NULL;

    VK_CHECK(vkCreateRenderPass(m_device, &renderPassCreateInfo, m_allocatorCb, &renderPass));

    m_renderPassCache.add(hashKey, renderPass);
    return renderPass;
}

StateCacheT<VkDescriptorSetLayout> &RendererVulkan::getDescriptorSetLayoutCache() {
    return m_descriptorSetLayoutCache;
}

void RendererVulkan::setInputLayout(
    VkPipelineVertexInputStateCreateInfo &vertexInputState,
    Bird::ProgramHandle program,
    const Bird::VertexBufferLayoutData &layoutData
) {
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.pNext = NULL;
    vertexInputState.flags = 0;

    VkVertexInputBindingDescription* bindingDescription =
        const_cast<VkVertexInputBindingDescription *>(vertexInputState.pVertexBindingDescriptions);

    VkVertexInputAttributeDescription *attributeDescriptions =
        const_cast<VkVertexInputAttributeDescription *>(
            vertexInputState.pVertexAttributeDescriptions
        );
    //        .binding = 0;
    bindingDescription->stride = layoutData.m_stride;
    bindingDescription->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    uint32_t attributeCount = 0;

    // Заполняем описания атрибутов
    for (uint32_t i = 0; i < layoutData.m_elementsCount && i < MAX_VERTEX_LAYOUT_ELEMENTS; i++) {
        const auto &element = layoutData.m_elements[i];
        VkVertexInputAttributeDescription &attribute = attributeDescriptions[attributeCount++];

        attribute.binding = 0; // Привязываем к 0-му биндингу
        attribute.location = i; // Локация в шейдере соответствует индексу
        attribute.offset =
            (i == 0)
                ? 0
                : attributeDescriptions[i - 1].offset +
                      layoutData.m_elements[i - 1].count *
                          VertexBufferElement::getSizeOfType(layoutData.m_elements[i - 1].type);

        // Преобразуем тип элемента в формат Vulkan
        switch (element.type) {
            case BufferElementType::Float:
                attribute.format = (element.count == 1)   ? VK_FORMAT_R32_SFLOAT
                                   : (element.count == 2) ? VK_FORMAT_R32G32_SFLOAT
                                   : (element.count == 3) ? VK_FORMAT_R32G32B32_SFLOAT
                                                          : VK_FORMAT_R32G32B32A32_SFLOAT;
                break;
            case BufferElementType::UnsignedInt:
                attribute.format = (element.count == 1)   ? VK_FORMAT_R32_UINT
                                   : (element.count == 2) ? VK_FORMAT_R32G32_UINT
                                   : (element.count == 3) ? VK_FORMAT_R32G32B32_UINT
                                                          : VK_FORMAT_R32G32B32A32_UINT;
                break;
            case BufferElementType::Int:
                attribute.format = (element.count == 1)   ? VK_FORMAT_R32_SINT
                                   : (element.count == 2) ? VK_FORMAT_R32G32_SINT
                                   : (element.count == 3) ? VK_FORMAT_R32G32B32_SINT
                                                          : VK_FORMAT_R32G32B32A32_SINT;
                break;
            case BufferElementType::UnsignedShort:
                attribute.format = (element.count == 1)   ? VK_FORMAT_R16_UINT
                                   : (element.count == 2) ? VK_FORMAT_R16G16_UINT
                                   : (element.count == 3) ? VK_FORMAT_R16G16B16_UINT
                                                          : VK_FORMAT_R16G16B16A16_UINT;
                break;
            case BufferElementType::UnsignedByte:
                attribute.format = element.normalized
                                       ? ((element.count == 1)   ? VK_FORMAT_R8_UNORM
                                          : (element.count == 2) ? VK_FORMAT_R8G8_UNORM
                                          : (element.count == 3) ? VK_FORMAT_R8G8B8_UNORM
                                                                 : VK_FORMAT_R8G8B8A8_UNORM)
                                       : ((element.count == 1)   ? VK_FORMAT_R8_UINT
                                          : (element.count == 2) ? VK_FORMAT_R8G8_UINT
                                          : (element.count == 3) ? VK_FORMAT_R8G8B8_UINT
                                                                 : VK_FORMAT_R8G8B8A8_UINT);
                break;
            default:
                NEST_ASSERT(false, "Unknown vertex buffer element type");
                break;
        }
    }

    vertexInputState.vertexBindingDescriptionCount = 1;
    vertexInputState.vertexAttributeDescriptionCount = attributeCount;
}

void RendererVulkan::setDepthStencilState(
    VkPipelineDepthStencilStateCreateInfo &depthStencilState, uint64_t state
) {
    depthStencilState = {};
    depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

    if (state & BIRD_STATE_DEPTH_TEST) {
        depthStencilState.depthTestEnable = VK_TRUE;
        depthStencilState.depthWriteEnable = VK_TRUE;
        depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;
    } else {
        depthStencilState.depthTestEnable = VK_FALSE;
        depthStencilState.depthWriteEnable = VK_FALSE;
    }

    depthStencilState.depthBoundsTestEnable = VK_FALSE;
    depthStencilState.minDepthBounds = 0.0f;
    depthStencilState.maxDepthBounds = 1.0f;

    depthStencilState.stencilTestEnable = VK_FALSE;
    depthStencilState.front = {};
    depthStencilState.back = {};
}

RendererType RendererVulkan::getRendererType() const {
    return RendererType::Vulkan;
}

void RendererVulkan::flip() {
    //    context->flip();
    VkPresentInfoKHR presentInfo;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = NULL;
    presentInfo.waitSemaphoreCount = 0;
    presentInfo.pWaitSemaphores = NULL;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapchain;
    presentInfo.pImageIndices = &m_imageIndex;
    presentInfo.pResults = NULL;
    VkResult result = vkQueuePresentKHR(m_graphicsQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        //        m_needToRefreshSwapchain = true;
    }
}

void RendererVulkan::createFrameBuffer(
    FrameBufferHandle handle, FrameBufferSpecification specification
) {
    m_frameBuffers[handle.id].m_delegate = this;
    m_frameBuffers[handle.id].create(specification);
}

void RendererVulkan::readFrameBuffer(
    FrameBufferHandle handle, int attachIndex, int x, int y, int width, int height, void *data
) {}

void RendererVulkan::deleteFrameBuffer(FrameBufferHandle handle) {}

void RendererVulkan::createProgram(ProgramHandle handle, ProgramCreate create) {
    m_shaders[handle.id].m_delegate = this;
    m_shaders[handle.id].create(create);
}

void RendererVulkan::deleteShader(ProgramHandle handle) {
    m_shaders[handle.id].terminate();
}

void RendererVulkan::createTexture(TextureHandle handle, TextureCreate create) {
    m_textures[handle.id].create(create);
}

void RendererVulkan::resizeTexture(TextureHandle handle, uint32_t width, uint32_t height) {
    m_textures[handle.id].resize(width, height);
}

void RendererVulkan::deleteTexture(TextureHandle handle) {
    m_textures[handle.id].terminate();
}

void RendererVulkan::createIndexBuffer(
    IndexBufferHandle handle,
    Foundation::Memory indices,
    BufferElementType elementType,
    size_t count
) {
    m_indexBuffers[handle.id].create(indices.data, elementType, count, false);
    indices.release();
}

void RendererVulkan::createDynamicIndexBuffer(
    IndexBufferHandle handle,
    Foundation::Memory indices,
    BufferElementType elementType,
    size_t count
) {}

void RendererVulkan::updateDynamicIndexBuffer(
    IndexBufferHandle handle, Foundation::Memory indices, size_t count
) {}

void RendererVulkan::deleteIndexBuffer(IndexBufferHandle handle) {}

void RendererVulkan::createVertexBuffer(
    VertexBufferHandle handle,
    Foundation::Memory data,
    uint32_t size,
    VertexLayoutHandle layoutHandle
) {
    m_vertexBuffers[handle.id].create(data.data, size, false);
    m_vertexBuffers[handle.id].setLayoutHandle(layoutHandle);
    data.release();
}

void RendererVulkan::createDynamicVertexBuffer(
    VertexBufferHandle handle,
    Foundation::Memory data,
    uint32_t size,
    VertexLayoutHandle layoutHandle
) {}

void RendererVulkan::updateDynamicVertexBuffer(
    VertexBufferHandle handle, Foundation::Memory data, uint32_t size
) {}

void RendererVulkan::deleteVertexBuffer(VertexBufferHandle handle) {}

void RendererVulkan::createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) {
    m_vertexLayouts[handle.id] = layout;
}

void RendererVulkan::deleteVertexLayout(VertexLayoutHandle handle) {}

void RendererVulkan::readTexture(Bird::TextureHandle handle, void *data) {
    m_textures[handle.id].readPixels(data);
}

void RendererVulkan::setUniform(const Uniform &uniform) {
    switch (uniform.type) {
        case UniformType::Sampler:
            return;
        case UniformType::Mat3:
            return;
        case UniformType::Mat4:
            return;
        case UniformType::Vec4:
            return;
        case UniformType::Count:
            return;
    }
    LOG_ERROR("UNIFORM TYPE IS UNDEFINED");
}

void RendererVulkan::setTexture(TextureHandle handle, uint32_t slot) {}

void RendererVulkan::viewChanged(View &view) {
    if (view.m_frameBuffer.isValid()) {
        m_frameBuffers[view.m_frameBuffer.id].bind();
    }
    if (!view.m_viewport.isZero()) {
        VkViewport viewport;
        viewport.x = view.m_viewport.origin.x;
        viewport.y = view.m_viewport.origin.y;
        viewport.width = view.m_viewport.size.width;
        viewport.height = -view.m_viewport.size.height; // ?
        viewport.minDepth = 0.0;
        viewport.maxDepth = 1.0;
        vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);
    }
    uint32_t rgba = view.m_clearColor;
    uint8_t r = rgba >> 24;
    uint8_t g = rgba >> 16;
    uint8_t b = rgba >> 8;
    uint8_t a = rgba >> 0;
    if (!view.m_frameBuffer.isValid()) {
        return;
    }
    for (auto clear : view.m_clearAttachments) {
        m_frameBuffers[view.m_frameBuffer.id].clearIntAttachment(
            clear.attachmentIndex, clear.value
        );
    }
}

void RendererVulkan::submit(Frame *frame, View *views) {
    BIRD_LOG("FRAME SUBMITTED. DRAW CALLS: {}", frame->getDrawCallsCount());
    if (frame->m_transientVbSize > 0) {
        m_vertexBuffers[frame->m_transientVb.handle.id].update(
            frame->m_transientVb.data, frame->m_transientVbSize
        );
    }
    if (frame->m_transientIbSize > 0) {
        m_indexBuffers[frame->m_transientIb.handle.id].update(
            frame->m_transientIb.data, frame->m_transientIbSize / 2
        );
    }

    if (!frame->getDrawCallsCount()) {
        return;
    }

    auto &currentFrame = m_swapchainFrames[m_frameNumber];

    VkSemaphore renderWait = currentFrame.imageAvailable;
    VkResult result = vkAcquireNextImageKHR(
        m_device, m_swapchain, UINT64_MAX, renderWait, VK_NULL_HANDLE, &m_imageIndex
    );

    VkCommandBufferBeginInfo commandBufferBeginInfo;
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.pNext = NULL;
    commandBufferBeginInfo.flags = 0 | VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    commandBufferBeginInfo.pInheritanceInfo = NULL;
    m_commandBuffer = currentFrame.commandBuffer;
    VK_CHECK(vkBeginCommandBuffer(m_commandBuffer, &commandBufferBeginInfo));

    ViewId viewId = -1;
    for (int i = 0; i < frame->getDrawCallsCount(); i++) {
        RenderDraw &draw = frame->getDrawCalls()[i];
        if (!draw.m_isSubmitted) {
            continue;
        }
        uint32_t rgba = views[draw.m_viewId].m_clearColor;
        uint8_t r = rgba >> 24;
        uint8_t g = rgba >> 16;
        uint8_t b = rgba >> 8;
        uint8_t a = rgba >> 0;
        VkClearValue clearValue;
        clearValue.color.uint32[0] = r;
        clearValue.color.uint32[1] = g;
        clearValue.color.uint32[2] = b;
        clearValue.color.uint32[3] = a;

        VkRenderPassBeginInfo renderPassBeginInfo;
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.pNext = NULL;
        renderPassBeginInfo.renderPass = m_renderPass;
        renderPassBeginInfo.framebuffer = currentFrame.framebuffer;
        renderPassBeginInfo.renderArea.offset.x = 0;
        renderPassBeginInfo.renderArea.offset.y = 0;
        renderPassBeginInfo.renderArea.extent = m_swapchainExtent;
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearValue;
        vkCmdBeginRenderPass(m_commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        if (draw.m_viewId != viewId) {
            viewId = draw.m_viewId;
            viewChanged(views[viewId]);
        }
        submit(&draw);
        vkCmdEndRenderPass(m_commandBuffer);
    }
    VK_CHECK(vkEndCommandBuffer(m_commandBuffer));
}

void RendererVulkan::submit(RenderDraw *draw) {
    // TODO: Capture time
    m_shaders[draw->m_shader.id].bind();

    VertexLayoutHandle layoutHandle =
        draw->m_vertexLayout.id != BIRD_INVALID_HANDLE
            ? draw->m_vertexLayout
            : m_vertexBuffers[draw->m_vertexBuffer.id].getLayoutHandle();
    NEST_ASSERT(layoutHandle.id != BIRD_INVALID_HANDLE, "Invalid handle");

    VertexBufferLayoutData layoutData = m_vertexLayouts[layoutHandle.id];
    VkPipeline pipeline = getPipeline(draw->m_state, draw->m_shader, layoutData);
//    vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    if (!draw->m_scissorRect.isZero()) {
        VkRect2D scissor;
        scissor.offset = {(int)draw->m_scissorRect.origin.x, (int)draw->m_scissorRect.origin.y};
        scissor.extent = {
            (uint32_t)draw->m_scissorRect.size.width, (uint32_t)draw->m_scissorRect.size.height
        };
        vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);
    } else {
    }
}

} // namespace Bird
