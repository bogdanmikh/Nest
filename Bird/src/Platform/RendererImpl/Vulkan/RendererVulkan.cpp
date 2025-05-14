//
// Created by Admin on 14.03.2022.
//

#include "RendererVulkan.hpp"
#include "Bird/Base.hpp"

#include <Foundation/Assert.hpp>
#include <Foundation/Allocator.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/PlatformDetection.hpp>
#include <Foundation/Assert.hpp>
#include "Bird/PlatformData.hpp"

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

namespace Bird {

RendererVulkan::RendererVulkan()
    : m_uselessVao(0)
    , m_frameBuffers()
    , m_shaders()
    , m_indexBuffers()
    , m_vertexLayouts()
    , m_vertexBuffers()
    , m_textures() {
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
    createSwapchain(getSize(), nullptr);
    createGraphicsPipeline();
}

void RendererVulkan::createInstance() {
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
    appInfo.apiVersion = VK_API_VERSION_1_2;
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

void RendererVulkan::createSwapchain(Size size, VkSwapchainKHR *oldSwapchain) {
    SwapchainSupportDetails support(std::move(querySwapchainSupport(m_physicalDevice, m_surface)));

    VkSurfaceFormatKHR format = chooseSwapchainSurfaceFormat(support.formats);

    VkPresentModeKHR presentMode = chooseSwapchainPresentMode(support.presentModes);

    VkExtent2D extent = chooseSwapchainExtent(size.width, size.height, support.capabilities);

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

    VK_CHECK(vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, nullptr, &m_swapchain));
    
    uint32_t count;
    vkGetSwapchainImagesKHR(m_device, m_swapchain, &count, nullptr);
    std::vector<VkImage> images(count);
    vkGetSwapchainImagesKHR(m_device, m_swapchain, &count, images.data());
    m_swapchainFrames.resize(images.size());
    for (int i = 0; i < images.size(); ++i) {
        VkImageViewCreateInfo imageViewInfo;
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
        VK_CHECK(vkCreateImageView(m_device, &imageViewInfo, nullptr, &m_swapchainFrames[i].imageView));
    }
    m_swapchainFormat = format.format;
    m_swapchainExtent = extent;

    maxFramesInFlight = m_swapchainFrames.size();
}

void RendererVulkan::createGraphicsPipeline() {

}

RendererVulkan::~RendererVulkan() {
    cleanupSwapchain();

    vkDestroyDevice(m_device, nullptr);
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
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
    for (const auto &frame: m_swapchainFrames) {
        vkDestroyImageView(m_device, frame.imageView, nullptr);
        vkDestroyFramebuffer(m_device, frame.framebuffer, nullptr);
        vkDestroyFence(m_device, frame.inFlight, nullptr);
        vkDestroySemaphore(m_device, frame.imageAvailable, nullptr);
        vkDestroySemaphore(m_device, frame.renderFinished, nullptr);
    }

    vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
}

RendererType RendererVulkan::getRendererType() const {
    return RendererType::Vulkan;
}

void RendererVulkan::flip() {
    context->flip();
}

void RendererVulkan::createFrameBuffer(
    FrameBufferHandle handle, FrameBufferSpecification specification
) {}

void RendererVulkan::readFrameBuffer(
    FrameBufferHandle handle, int attachIndex, int x, int y, int width, int height, void *data
) {}

void RendererVulkan::deleteFrameBuffer(FrameBufferHandle handle) {}

void RendererVulkan::createProgram(ProgramHandle handle, ProgramCreate create) {}

void RendererVulkan::deleteShader(ProgramHandle handle) {}

void RendererVulkan::createTexture(TextureHandle handle, TextureCreate create) {}

void RendererVulkan::resizeTexture(TextureHandle handle, uint32_t width, uint32_t height) {}

void RendererVulkan::deleteTexture(TextureHandle handle) {}

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
) {}

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

void RendererVulkan::createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) {}

void RendererVulkan::deleteVertexLayout(VertexLayoutHandle handle) {}

void RendererVulkan::readTexture(Bird::TextureHandle handle, void *data) {}

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

void RendererVulkan::submit(Frame *frame, View *views) {}

void RendererVulkan::viewChanged(View &view) {}

void RendererVulkan::submit(RenderDraw *draw) {
    // TODO: Capture time
}

} // namespace Bird
