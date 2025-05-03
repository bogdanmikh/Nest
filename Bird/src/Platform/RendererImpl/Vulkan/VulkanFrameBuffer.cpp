#include "VulkanFrameBuffer.hpp"
#include "RendererVulkan.hpp"

namespace Bird {

static inline bool isDepthFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::DEPTH24STENCIL8:
            return true;
        default:
            return false;
    }
}

void VulkanFrameBuffer::checkStatus() {}

VulkanFrameBuffer::VulkanFrameBuffer()
    : spec()
    , m_id(-1) {}

void VulkanFrameBuffer::create(RendererOpenGL *renderer, FrameBufferSpecification specification) {}

void VulkanFrameBuffer::bind() {}

void VulkanFrameBuffer::unbind() {}

void VulkanFrameBuffer::terminate() {}

void VulkanFrameBuffer::clearIntAttachment(int index, int value) {}

void VulkanFrameBuffer::clearUIntAttachment(int index, uint32_t value) {}

void VulkanFrameBuffer::readPixels(
    RendererOpenGL *renderer, int index, int x, int y, int width, int height, void *data
) {}

} // namespace Bird