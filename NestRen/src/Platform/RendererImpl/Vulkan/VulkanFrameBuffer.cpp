#include "VulkanFrameBuffer.hpp"
#include "RendererVulkan.hpp"

#include "VulkanBase.hpp"

namespace NestRen {

static inline bool isDepthFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::DEPTH24STENCIL8:
            return true;
        default:
            return false;
    }
}

void VulkanFrameBuffer::checkStatus() {
    NEST_ASSERT(
        glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
        "Framebuffer is incomplete!"
    );
}

VulkanFrameBuffer::VulkanFrameBuffer()
    : spec()
    , m_id(-1) {}

void VulkanFrameBuffer::create(FrameBufferSpecification specification) {
    NEST_ASSERT(m_id == -1, "FRAMEBUFFER ALREADY CREATED");
    // this->specification = specification;
    GL_CALL(glGenFramebuffers(1, &m_id));
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
    for (int i = 0; i < specification.num; i++) {
        FrameBufferAttachment &attach = specification.attachments[i];
        VulkanTexture &texture = RendererVulkan::s_instance->getTexture(attach.handle);
        int attachmentType;
        if (isDepthFormat(texture.getFormat())) {
            attachmentType = GL_DEPTH_ATTACHMENT;
        } else {
            attachmentType = GL_COLOR_ATTACHMENT0 + i;
        }
        GLuint textureId = texture.getId();
        GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, textureId, 0)
        );
    }
    checkStatus();
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void VulkanFrameBuffer::bind() {
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
}

void VulkanFrameBuffer::unbind() {
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void VulkanFrameBuffer::terminate() {
    NEST_ASSERT(m_id != -1, "FRAMEBUFFER ALREADY DELETED");
    GL_CALL(glDeleteFramebuffers(1, &m_id));
    m_id = -1;
}

} // namespace NestRen