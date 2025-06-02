#pragma once

#include "Bird/Base.hpp"

#include <vector>
#include <vulkan/vulkan.h>

namespace Bird {

class RendererOpenGL;

class VulkanFrameBufferDelegate {
public:
    virtual ~VulkanFrameBufferDelegate() = default;
    virtual VkRenderPass getRenderPass(uint32_t num, const FrameBufferAttachment *attachments) = 0;
};

class VulkanFrameBuffer {
public:
    VulkanFrameBuffer();
    void create(FrameBufferSpecification specification);
    void terminate();
    void bind();
    void unbind();
    void readPixels(
        RendererOpenGL *renderer, int index, int x, int y, int width, int height, void *data
    );
    void clearIntAttachment(int index, int value);
    void clearUIntAttachment(int index, uint32_t value);

    VkRenderPass m_renderPass;
    VulkanFrameBufferDelegate *m_delegate;

private:
    void checkStatus();

    FrameBufferSpecification spec;
    uint32_t m_id;
};

} // namespace Bird
