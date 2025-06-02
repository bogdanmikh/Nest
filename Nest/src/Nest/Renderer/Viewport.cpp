//
// Created by Bogdan on 29.09.2024.
//

#include "Nest/Renderer/Viewport.hpp"
#include "Nest/Application/Application.hpp"

namespace Nest {

// TODO: сделать систему раздачи viewId viewport-ам
Bird::ViewId Viewport::m_lastViewId = 1;

Viewport::Viewport()
    : m_viewId(m_lastViewId) {
    m_lastViewId++;
}

Viewport::~Viewport() {
    deleteFrameBuffer(m_frameBuffer);
    deleteTexture(m_colorAttachment);
    deleteTexture(m_depthAttachment);
}

void Viewport::init() {
    auto windowSize = Nest::Application::get()->getWindow()->getSize();
    auto windowDpi = Nest::Application::get()->getWindow()->getDpi();
    m_windowSize = {windowSize.x * windowDpi.x, windowSize.y * windowDpi.y};

    Bird::TextureCreate create;
    create.m_data = Foundation::Memory(nullptr);
    create.m_format = Bird::TextureFormat::RGBA8;
    create.m_width = m_windowSize.width;
    create.m_height = m_windowSize.height;
    create.m_wrap = Bird::CLAMP_TO_BORDER;
    create.m_borderColor = 0xFFFFFFFF;
    m_colorAttachment = Bird::createTexture(create);
    create.m_format = Bird::TextureFormat::DEPTH32STENCIL8;
    m_depthAttachment = Bird::createTexture(create);
    Bird::FrameBufferAttachment attachments[] = {m_colorAttachment, m_depthAttachment};
    m_fbSpecification = Bird::FrameBufferSpecification(attachments, 2);
    m_frameBuffer = Bird::createFrameBuffer(m_fbSpecification);
    Bird::setViewport(m_viewId, Bird::Rect(0, 0, m_windowSize.width, m_windowSize.height));
    Bird::setViewClear(m_viewId, 0x12212bff);
    Bird::setViewFrameBuffer(m_viewId, m_frameBuffer);
}

void Viewport::initWithSize(Nest::Vec2 size) {
    m_windowSize = size;
    Bird::TextureCreate create;
    create.m_data = Foundation::Memory(nullptr);
    create.m_format = Bird::TextureFormat::RGBA8;
    create.m_width = m_windowSize.width;
    create.m_height = m_windowSize.height;
    m_colorAttachment = Bird::createTexture(create);
    create.m_format = Bird::TextureFormat::DEPTH32STENCIL8;
    m_depthAttachment = Bird::createTexture(create);
    Bird::FrameBufferAttachment attachments[] = {m_colorAttachment, m_depthAttachment};
    m_fbSpecification = Bird::FrameBufferSpecification(attachments, 2);
    m_frameBuffer = Bird::createFrameBuffer(m_fbSpecification);
    Bird::setViewport(m_viewId, Bird::Rect(0, 0, m_windowSize.width, m_windowSize.height));
    Bird::setViewClear(m_viewId, 0x12212bff);
    Bird::setViewFrameBuffer(m_viewId, m_frameBuffer);
}

void Viewport::update() {
    auto currentSize = Nest::Application::get()->getWindow()->getSize();
    auto currentDpi = Nest::Application::get()->getWindow()->getDpi();
    currentSize.x *= currentDpi.x;
    currentSize.y *= currentDpi.y;
    if (m_windowSize != currentSize) {
        m_windowSize = currentSize;
        updateViewportSize(m_windowSize);
    }
}

void Viewport::updateViewportSize(Vec2 size) {
    if (size.width < 1 || size.height < 1) {
        return;
    }
    m_windowSize = size;
    Bird::setViewport(m_viewId, Bird::Rect(0, 0, size.width, size.height));
    // COLOR ATTACHMENT
    Bird::resizeTexture(m_fbSpecification.attachments[0].handle, size.width, size.height);
    // DEPTH ATTACHMENT
    Bird::resizeTexture(m_fbSpecification.attachments[1].handle, size.width, size.height);
    Bird::deleteFrameBuffer(m_frameBuffer);
    m_frameBuffer = Bird::createFrameBuffer(m_fbSpecification);
    Bird::setViewFrameBuffer(m_viewId, m_frameBuffer);
}

Bird::ViewId Viewport::getViewId() {
    return m_viewId;
}

Bird::TextureHandle Viewport::getTextureHandle() {
    return m_colorAttachment;
}

Bird::TextureHandle Viewport::getDepthHandle() {
    return m_depthAttachment;
}

Vec2 Viewport::getSize() {
    return m_windowSize;
}

} // namespace Nest