#include "Nest/Renderer/FrameBuffer.hpp"
#include "glad/glad.h"
#include <Nest/Renderer/ErrorsOpenGL.hpp>

FrameBuffer::FrameBuffer(const Vec2 &size)
    : m_RendererID(0) {
    GL_CALL(glGenFramebuffers(1, &m_RendererID));
    bind();
    texture.createFromData(size.width, size.height, 4, nullptr);
    GL_CALL(glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getRendererId(), 0
    ));
    checkStatus();
    bind();
}

void FrameBuffer::resize(const Vec2 &size) {
    GL_CALL(glDeleteFramebuffers(1, &m_RendererID));
    GL_CALL(glGenFramebuffers(1, &m_RendererID));
    bind();
    texture.createFromData(size.width, size.height, 4, nullptr);
    GL_CALL(glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.getRendererId(), 0
    ));
    checkStatus();
    bind();
}

uint32_t FrameBuffer::getRendererIdTexture() {
    return texture.getRendererId();
}

Vec2 FrameBuffer::getSize() {
    return texture.getSize();
}

FrameBuffer::~FrameBuffer() {
    GL_CALL(glDeleteFramebuffers(1, &m_RendererID));
}

void FrameBuffer::bind() const {
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
}

void FrameBuffer::unbind() const {
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::checkStatus() {
    VIVO_ASSERT(
        glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
        "FRAGMENTBUFFER NOT COMPLETE"
    );
}

Texture *FrameBuffer::getTexture() {
    return &texture;
}