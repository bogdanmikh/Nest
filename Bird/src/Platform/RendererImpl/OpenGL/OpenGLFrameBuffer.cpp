#include "OpenGLFrameBuffer.hpp"
#include "RendererOpenGL.hpp"
#include "Texture/TextureFormat.hpp"

#include "OpenGLBase.hpp"
#include "Extensions/OpenGLExtensions.hpp"

namespace Bird {

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
    spec = specification;
    // this->specification = specification;
    GL_CALL(glGenFramebuffers(1, &m_id));
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
    std::vector<GLenum> colorAttachments;
    for (int i = 0; i < specification.num; i++) {
        FrameBufferAttachment &attach = specification.attachments[i];
        OpenGLTexture &texture = RendererOpenGL::s_instance->getTexture(attach.handle);
        int attachmentType;
        if (isDepthFormat(texture.getFormat())) {
            attachmentType = GL_DEPTH_ATTACHMENT;
        } else {
            attachmentType = GL_COLOR_ATTACHMENT0 + i;
            colorAttachments.emplace_back(attachmentType);
        }
        GLuint textureId = texture.getId();
        GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, textureId, 0)
        );
    }
    GL_CALL(glDrawBuffers(colorAttachments.size(), colorAttachments.data()));
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

void OpenGLFrameBuffer::clearIntAttachment(int index, int value) {
    NEST_ASSERT(index < spec.num, "INVALID ATTACHMENT INDEX");
    bind();
    glClearBufferiv(GL_COLOR, index, &value);
}

void OpenGLFrameBuffer::readPixels(int index, int x, int y, int width, int height, void *data) {
    bind();
    OpenGLTexture &texture = RendererOpenGL::s_instance->getTexture(spec.attachments[index].handle);
    TextureFormat format = texture.getFormat();
    int attachmentType;
    if (isDepthFormat(format)) {
        attachmentType = GL_DEPTH_ATTACHMENT;
    } else {
        attachmentType = GL_COLOR_ATTACHMENT0 + index;
    }
    GL_CALL(glReadBuffer(attachmentType));

    GLenum fmt = s_textureFormat[format].m_internalFmt;
    GLenum type = s_textureFormat[format].m_type;
    GL_CALL(glReadPixels(x, y, width, height, fmt, type, data));
}

} // namespace Bird