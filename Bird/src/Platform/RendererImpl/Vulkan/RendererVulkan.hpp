//
// Created by Admin on 14.03.2022.
//

#pragma once
#include <Bird/Base.hpp>
#include "Bird/RendererI.hpp"
#include "VulkanFrameBuffer.hpp"
#include "VulkanShader.hpp"
#include "VulkanTexture.hpp"
#include "VulkanIndexBuffer.hpp"
#include "VulkanVertexBuffer.hpp"
#include "Bird/GraphicsContext.hpp"
#include "VulkanBase.hpp"

#define VK_DESTROY                                                                                 \
    VK_DESTROY_FUNC(Buffer);                                                                       \
    VK_DESTROY_FUNC(CommandPool);                                                                  \
    VK_DESTROY_FUNC(DescriptorPool);                                                               \
    VK_DESTROY_FUNC(DescriptorSetLayout);                                                          \
    VK_DESTROY_FUNC(Fence);                                                                        \
    VK_DESTROY_FUNC(Framebuffer);                                                                  \
    VK_DESTROY_FUNC(Image);                                                                        \
    VK_DESTROY_FUNC(ImageView);                                                                    \
    VK_DESTROY_FUNC(Sampler);                                                                      \
    VK_DESTROY_FUNC(Pipeline);                                                                     \
    VK_DESTROY_FUNC(PipelineCache);                                                                \
    VK_DESTROY_FUNC(PipelineLayout);                                                               \
    VK_DESTROY_FUNC(RenderPass);                                                                   \
    VK_DESTROY_FUNC(Semaphore);                                                                    \
    VK_DESTROY_FUNC(ShaderModule);                                                                 \
    VK_DESTROY_FUNC(SwapchainKHR);

#define VK_DESTROY_FUNC(_name)                                                                     \
    struct Vk##_name {                                                                             \
        ::Vk##_name vk;                                                                            \
        Vk##_name() {}                                                                             \
        Vk##_name(::Vk##_name _vk)                                                                 \
            : vk(_vk) {}                                                                           \
        operator ::Vk##_name() {                                                                   \
            return vk;                                                                             \
        }                                                                                          \
        operator ::Vk##_name() const {                                                             \
            return vk;                                                                             \
        }                                                                                          \
        ::Vk##_name *operator&() {                                                                 \
            return &vk;                                                                            \
        }                                                                                          \
        const ::Vk##_name *operator&() const {                                                     \
            return &vk;                                                                            \
        }                                                                                          \
    };                                                                                             \
    NEST_STATIC_ASSERT(sizeof(::Vk##_name) == sizeof(Vk##_name));                                  \
    void vkDestroy(Vk##_name &)
VK_DESTROY
#undef VK_DESTROY_FUNC

namespace Bird {

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

class RendererVulkan : public RendererI {
public:
    RendererVulkan();
    ~RendererVulkan() override;
    RendererType getRendererType() const override;
    void flip() override;
    void
    createFrameBuffer(FrameBufferHandle handle, FrameBufferSpecification specification) override;
    void readFrameBuffer(
        Bird::FrameBufferHandle handle,
        int attachIndex,
        int x,
        int y,
        int width,
        int height,
        void *data
    ) override;
    void deleteFrameBuffer(FrameBufferHandle handle) override;
    void createProgram(ProgramHandle handle, ProgramCreate create) override;
    void deleteShader(ProgramHandle handle) override;
    void createTexture(TextureHandle handle, TextureCreate create) override;
    void resizeTexture(TextureHandle handle, uint32_t width, uint32_t height) override;
    void deleteTexture(TextureHandle handle) override;
    void createIndexBuffer(
        IndexBufferHandle handle,
        Foundation::Memory indices,
        BufferElementType elementType,
        size_t count
    ) override;
    void createDynamicIndexBuffer(
        IndexBufferHandle handle,
        Foundation::Memory indices,
        BufferElementType elementType,
        size_t count
    ) override;
    void updateDynamicIndexBuffer(
        IndexBufferHandle handle, Foundation::Memory indices, size_t count
    ) override;
    void deleteIndexBuffer(IndexBufferHandle handle) override;
    void createVertexBuffer(
        VertexBufferHandle handle,
        Foundation::Memory data,
        uint32_t size,
        VertexLayoutHandle layoutHandle
    ) override;
    void createDynamicVertexBuffer(
        VertexBufferHandle handle,
        Foundation::Memory data,
        uint32_t size,
        VertexLayoutHandle layoutHandle
    ) override;
    void updateDynamicVertexBuffer(
        VertexBufferHandle handle, Foundation::Memory data, uint32_t size
    ) override;
    void deleteVertexBuffer(VertexBufferHandle handle) override;
    void createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) override;
    void deleteVertexLayout(VertexLayoutHandle handle) override;
    void readTexture(Bird::TextureHandle handle, void *data) override;
    void setUniform(const Uniform &uniform) override;
    void setTexture(TextureHandle handle, uint32_t slot) override;
    void submit(Frame *frame, View *views) override;
    VulkanTexture &getTexture(TextureHandle handle) {
        return m_textures[handle.id];
    }

private:
    void viewChanged(View &view);
    void submit(RenderDraw *draw);

    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void setupAllocator();
    void createSwapchain(Size size, VkSwapchainKHR *oldSwapchain);
    void cleanupSwapchain();

    void setDeviceForVKObjects();

    VkPipeline getPipeline(
        uint64_t state,
        uint8_t numStreams,
        const Bird::VertexBufferLayoutData **layouts,
        Bird::ProgramHandle program,
        uint8_t numInstanceData
    );

    VkPipelineMultisampleStateCreateInfo getMultisampleState(uint32_t stateFlags);

    void setInputLayout(
        VkPipelineVertexInputStateCreateInfo &vertexInputState,
        uint8_t numStream,
        const Bird::VertexLayoutHandle **layout,
        const VulkanShader &shader,
        uint8_t numInstanceData
    );
    struct QueueFamilyIndices {
        const uint32_t invalidValue = UINT32_MAX;
        uint32_t graphicsFamily = invalidValue;
        uint32_t presentFamily = invalidValue;

        bool isComplete() {
            return graphicsFamily != invalidValue && presentFamily != invalidValue;
        }
    };

    struct SwapchainFrame {
        VkImage image;
        VkImageView imageView;
        VkFramebuffer framebuffer;
        VkCommandBuffer commandBuffer;
        VkSemaphore imageAvailable, renderFinished;
        VkFence inFlight;
    };

    QueueFamilyIndices findQueueFamilies();

    uint32_t m_uselessVao;
    GraphicsContext *context;
    VulkanFrameBuffer m_frameBuffers[MAX_FRAME_BUFFERS];
    VulkanShader m_shaders[MAX_SHADERS];
    VulkanIndexBuffer m_indexBuffers[MAX_INDEX_BUFFERS];
    VertexBufferLayoutData m_vertexLayouts[MAX_BUFFER_LAYOUTS];
    VulkanVertexBuffer m_vertexBuffers[MAX_VERTEX_BUFFERS];
    VulkanTexture m_textures[MAX_TEXTURES];
    // Instance-related variables
    VkInstance m_instance;
    VkSurfaceKHR m_surface;
    VkDebugUtilsMessengerEXT m_debugMessenger;
    // Device-related variables
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_device;
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
    VkSwapchainKHR m_swapchain;
    std::vector<SwapchainFrame> m_swapchainFrames;
    VkFormat m_swapchainFormat;
    VkExtent2D m_swapchainExtent;

    int32_t maxFramesInFlight, frameNumber;

    uint32_t apiVersion;

    //    std::unordered_map
};

} // namespace Bird
