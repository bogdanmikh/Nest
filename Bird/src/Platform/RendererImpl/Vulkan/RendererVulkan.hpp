//
// Created by Admin on 14.03.2022.
//

#pragma once

#include "Bird/Base.hpp"
#include "Bird/RendererI.hpp"
#include "VulkanFrameBuffer.hpp"
#include "VulkanShader.hpp"
#include "VulkanTexture.hpp"
#include "VulkanIndexBuffer.hpp"
#include "VulkanVertexBuffer.hpp"
#include "Bird/GraphicsContext.hpp"
#include "VulkanBase.hpp"

namespace Bird {

class RendererVulkan : public RendererI, VulkanFrameBufferDelegate, VulkanShaderDelegate {
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

#pragma region VulkanFrameBufferDelegate
    VkRenderPass getRenderPass(uint32_t num, const FrameBufferAttachment *attachments) override;
#pragma endregion

#pragma region VulkanShaderDelegate
    StateCacheT<VkDescriptorSetLayout> &getDescriptorSetLayoutCache() override;
#pragma endregion

    VkResult allocateMemory(
        const VkMemoryRequirements *requirements,
        VkMemoryPropertyFlags propertyFlags,
        VkDeviceMemory *memory
    ) const;

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
    void createFence();
    void createSemaphores();
    void createSwapchainFramebuffer();

    void releaseSwapchainFramebuffer();
    void createSwapchainRenderPass();
    void releaseSwapchainRenderPass();
    void initSwapchainImageLayout();

    void createCommandPool();

    int32_t
    selectMemoryType(uint32_t memoryTypeBits, uint32_t propertyFlags, int32_t startIndex = 0) const;

    VkPipeline getPipeline(
        uint64_t state, Bird::ProgramHandle program, const Bird::VertexBufferLayoutData &layoutData
    );

    VkPipelineMultisampleStateCreateInfo getMultisampleState(uint32_t stateFlags);
    void setInputLayout(
        VkPipelineVertexInputStateCreateInfo &vertexInputState,
        Bird::ProgramHandle program,
        const Bird::VertexBufferLayoutData &layoutData
    );

    void
    setDepthStencilState(VkPipelineDepthStencilStateCreateInfo &depthStencilState, uint64_t state);
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
    VkFence m_fence;
    VkSwapchainKHR m_swapchain;

    FrameBufferHandle m_frameBufferHandle;

#define NUM_SWAPCHAIN_IMAGE 4
    SwapchainFrame m_swapchainFrames[NUM_SWAPCHAIN_IMAGE];
    VkCommandBuffer m_commandBuffer;

    VkImage m_depthStencilImage;
    VkImageView m_depthStencilImageView;
    VkFormat m_depthStencilFormat;
    VkRenderPass m_renderPass;

    VkCommandPool m_commandPool;
    VkDeviceMemory m_depthStencilMemory;
    VkPhysicalDeviceMemoryProperties m_memoryProperties;

    StateCacheT<VkPipeline> m_pipelineStateCache;
    StateCacheT<VkDescriptorSetLayout> m_descriptorSetLayoutCache;
    StateCacheT<VkRenderPass> m_renderPassCache;
    StateCacheT<VkSampler> m_samplerCache;

    VkFormat m_swapchainFormat;
    VkExtent2D m_swapchainExtent;

    uint32_t m_frameNumber;
    uint32_t m_numSwapchainImages;
    uint32_t m_imageIndex;

    uint32_t apiVersion;

    VkAllocationCallbacks *m_allocatorCb;
};

} // namespace Bird
