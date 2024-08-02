#include "OpenGLFrameBuffer.hpp"
#include "RendererVulkan.hpp"

#include "OpenGLBase.hpp"

namespace NestRen {

static inline bool isDepthFormat(TextureFormat format) {
    switch (format) {
        case TextureFormat::DEPTH24STENCIL8:
            return true;
        default:
            return false;
    }
}

void OpenGLFrameBuffer::checkStatus() {
    NEST_ASSERT(
        glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
        "Framebuffer is incomplete!"
    );
}

OpenGLFrameBuffer::OpenGLFrameBuffer()
    : spec()
    , m_id(-1) {}

void OpenGLFrameBuffer::create(FrameBufferSpecification specification) {
    NEST_ASSERT(m_id == -1, "FRAMEBUFFER ALREADY CREATED");
    // this->specification = specification;
    GL_CALL(glGenFramebuffers(1, &m_id));
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
    for (int i = 0; i < specification.num; i++) {
        FrameBufferAttachment &attach = specification.attachments[i];
        OpenGLTexture &texture = RendererOpenGL::s_instance->getTexture(attach.handle);
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

void OpenGLFrameBuffer::bind() {
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
}

void OpenGLFrameBuffer::unbind() {
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void OpenGLFrameBuffer::terminate() {
    NEST_ASSERT(m_id != -1, "FRAMEBUFFER ALREADY DELETED");
    GL_CALL(glDeleteFramebuffers(1, &m_id));
    m_id = -1;
}

} // namespace NestRen