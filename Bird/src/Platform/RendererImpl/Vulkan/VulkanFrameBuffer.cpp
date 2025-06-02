#include "VulkanFrameBuffer.hpp"
#include "RendererVulkan.hpp"

namespace Bird {

static inline bool isDepthFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::DEPTH32STENCIL8:
            return true;
        default:
            return false;
    }
}

void VulkanFrameBuffer::checkStatus() {}

VulkanFrameBuffer::VulkanFrameBuffer()
    : spec()
    , m_id(-1) {}

void VulkanFrameBuffer::create(FrameBufferSpecification specification) {
    spec = specification;
    VkDevice device = device;
    VkAllocationCallbacks *allocatorCb = allocatorCb;
    VkRenderPass renderPass = m_delegate->getRenderPass(spec.num, spec.attachments);

    //    VulkanTexture& firstTexture = s_renderVK->m_textures[m_attachment[0].handle.idx];
    //    ::VkImageView textureImageViews[BGFX_CONFIG_MAX_FRAME_BUFFER_ATTACHMENTS];
    //
    //    m_depth.idx = bx::kInvalidHandle;
    //    m_num = 0;
    //    for (uint8_t ii = 0; ii < m_numAttachment; ++ii)
    //    {
    //        TextureVK& texture = s_renderVK->m_textures[m_attachment[ii].handle.idx];
    //        textureImageViews[ii] = texture.m_textureImageView;
    //        if (texture.m_aspectMask & VK_IMAGE_ASPECT_COLOR_BIT)
    //        {
    //            m_texture[m_num] = m_attachment[ii].handle;
    //            m_num++;
    //        }
    //        else if (texture.m_aspectMask & VK_IMAGE_ASPECT_DEPTH_BIT)
    //        {
    //            m_depth = m_attachment[ii].handle;
    //        }
    //    }
    //
    //    VkFramebufferCreateInfo fci;
    //    fci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    //    fci.pNext = NULL;
    //    fci.flags = 0;
    //    fci.renderPass      = renderPass;
    //    fci.attachmentCount = m_numAttachment;
    //    fci.pAttachments    = textureImageViews;
    //    fci.width  = firstTexture.m_width >> m_attachment[0].mip;
    //    fci.height = firstTexture.m_height >> m_attachment[0].mip;
    //    fci.layers = firstTexture.m_numSides;
    //    VK_CHECK( vkCreateFramebuffer(device, &fci, allocatorCb, &m_framebuffer) );
    //    m_renderPass = renderPass;
}

void VulkanFrameBuffer::bind() {}

void VulkanFrameBuffer::unbind() {}

void VulkanFrameBuffer::terminate() {}

void VulkanFrameBuffer::clearIntAttachment(int index, int value) {}

void VulkanFrameBuffer::clearUIntAttachment(int index, uint32_t value) {}

void VulkanFrameBuffer::readPixels(
    RendererOpenGL *renderer, int index, int x, int y, int width, int height, void *data
) {}

} // namespace Bird