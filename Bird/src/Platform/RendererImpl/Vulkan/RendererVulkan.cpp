//
// Created by Admin on 14.03.2022.
//

#include "RendererVulkan.hpp"
#include "Bird/Base.hpp"

#include <Foundation/Assert.hpp>
#include <Foundation/Allocator.hpp>
#include <Foundation/Logger.hpp>
#include <vulkan/vulkan.h>
#include <sstream>
#include <Foundation/PlatformDetection.hpp>
#include <Foundation/Assert.hpp>
#include "Bird/PlatformData.hpp"

#ifdef PLATFORM_IOS
#    include "Platform/RendererImpl/Context/GlesContext.hpp"
#elif defined(PLATFORM_ANDROID)
#    include "Platform/RendererImpl/Context/AndroidContext.hpp"
#elif defined(PLATFORM_DESKTOP)
#    include "Platform/RendererImpl/Context/VulkanContext.hpp"
#endif

const char *getResultToString(VkResult result) {
    switch (result) {
        case VK_SUCCESS:
            return "VK_SUCCESS";
        case VK_NOT_READY:
            return "VK_NOT_READY";
        case VK_TIMEOUT:
            return "VK_TIMEOUT";
        case VK_EVENT_SET:
            return "VK_EVENT_SET";
        case VK_EVENT_RESET:
            return "VK_EVENT_RESET";
        case VK_INCOMPLETE:
            return "VK_INCOMPLETE";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST:
            return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "VK_ERROR_MEMORY_MAP_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "VK_ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "VK_ERROR_TOO_MANY_OBJECTS";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "VK_ERROR_FORMAT_NOT_SUPPORTED";
        case VK_ERROR_FRAGMENTED_POOL:
            return "VK_ERROR_FRAGMENTED_POOL";
        case VK_ERROR_UNKNOWN:
            return "VK_ERROR_UNKNOWN";
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            return "VK_ERROR_OUT_OF_POOL_MEMORY";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
        case VK_ERROR_FRAGMENTATION:
            return "VK_ERROR_FRAGMENTATION";
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
            return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
        case VK_PIPELINE_COMPILE_REQUIRED:
            return "VK_PIPELINE_COMPILE_REQUIRED";
        case VK_ERROR_SURFACE_LOST_KHR:
            return "VK_ERROR_SURFACE_LOST_KHR";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
        case VK_SUBOPTIMAL_KHR:
            return "VK_SUBOPTIMAL_KHR";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "VK_ERROR_OUT_OF_DATE_KHR";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
        case VK_ERROR_VALIDATION_FAILED_EXT:
            return "VK_ERROR_VALIDATION_FAILED_EXT";
        case VK_ERROR_INVALID_SHADER_NV:
            return "VK_ERROR_INVALID_SHADER_NV";
        case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
            return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
            return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
        case VK_ERROR_NOT_PERMITTED_KHR:
            return "VK_ERROR_NOT_PERMITTED_KHR";
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
            return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
        case VK_THREAD_IDLE_KHR:
            return "VK_THREAD_IDLE_KHR";
        case VK_THREAD_DONE_KHR:
            return "VK_THREAD_DONE_KHR";
        case VK_OPERATION_DEFERRED_KHR:
            return "VK_OPERATION_DEFERRED_KHR";
        case VK_OPERATION_NOT_DEFERRED_KHR:
            return "VK_OPERATION_NOT_DEFERRED_KHR";
        case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
            return "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
            return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
        case VK_INCOMPATIBLE_SHADER_BINARY_EXT:
            return "VK_INCOMPATIBLE_SHADER_BINARY_EXT";
        default:
            return "UNKNOWN_VK_RESULT";
    }
}

#if BIRD_LOG_ENABLED == 1
#    define VK_CHECK(result)                                                                       \
        if (result != VK_SUCCESS) {                                                                \
            LOG_ERROR("Vulkan error: {}", getResultToString(result));                              \
            assert(false);                                                                         \
        }
#else
#    define VK_CHECK(result)                                                                       \
        if (result != VK_SUCCESS) {                                                                \
            assert(false);                                                                         \
        }
#endif

#include <vector>
#include <cstring>

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

    std::array<bool, 5> typeGPU{false};
    // 0 - Discrete GPU, 1 - Integrated GPU, 2 - Virtual GPU, 3 - CPU, 4 - Other
    switch (properties.deviceType) {
        case (VK_PHYSICAL_DEVICE_TYPE_CPU):
            typeGPU[3] = true;
        case (VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU):
            typeGPU[0] = true;
        case (VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU):
            typeGPU[1] = true;
        case (VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU):
            typeGPU[2] = true;
        default:
            typeGPU[4] = true;
    }
    return typeGPU;
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
    if (!glfwVulkanSupported()) {
        LOG_CRITICAL("GLFW NOT SUPPORT VULKAN!");
        exit(1);
    }
    context = F_NEW(Foundation::getAllocator(), VulkanContext);
#endif
    context->create();
    makeInstance();
    //    makeDevice();
}

void RendererVulkan::makeInstance() {
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
    appInfo.applicationVersion = version;
    //    appInfo.apiVersion = version;
    appInfo.apiVersion = VK_MAKE_VERSION(1, 1, 0);
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
    layers.push_back("VK_LAYER_KHRONOS_validation");
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
#ifdef PLATFORM_DESKTOP
    VkSurfaceKHR surface;
    auto *window = static_cast<GLFWwindow *>(PlatformData::get()->nativeWindowHandle);
    VK_CHECK(glfwCreateWindowSurface(m_instance, window, nullptr, &surface));

    m_surface = surface;

    BIRD_LOG("Successfully abstracted GLFW surface for Vulkan");
#endif
}

void RendererVulkan::makeDevice() {
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
}

RendererVulkan::~RendererVulkan() {
    vkDestroyInstance(m_instance, nullptr);
    F_DELETE(Foundation::getAllocator(), context);
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
