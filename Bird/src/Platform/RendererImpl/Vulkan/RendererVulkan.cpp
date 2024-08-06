//
// Created by Admin on 14.03.2022.
//

#include "RendererVulkan.hpp"
#include "VulkanBase.hpp"

#include <Foundation/Assert.hpp>
#include <Foundation/Allocator.hpp>
#include <Foundation/Logger.hpp>

#ifdef PLATFORM_IOS
#    include "Platform/RendererImpl/Context/GlesContext.hpp"
#elif defined(PLATFORM_DESKTOP)
#    include "Platform/RendererImpl/Context/VulkanContext.hpp"
#endif

namespace Bird {

RendererVulkan *RendererVulkan::s_instance;

void gpuErrorCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam
) {
    LOG_INFO(message);
    LOG_CRITICAL("Vulkan ERROR");
}

const char *getGLErrorStr(GLenum err) {
    switch (err) {
        case GL_NO_ERROR:
            return "No error";
        case GL_INVALID_ENUM:
            return "Invalid enum";
        case GL_INVALID_VALUE:
            return "Invalid value";
        case GL_INVALID_OPERATION:
            return "Invalid operation";
            // case GL_STACK_OVERFLOW:    return "Stack overflow";
            // case GL_STACK_UNDERFLOW:   return "Stack underflow";
        case GL_OUT_OF_MEMORY:
            return "Out of memory";
        default:
            return "Unknown error";
    }
}

RendererVulkan::RendererVulkan() {
    s_instance = this;
#ifdef PLATFORM_IOS
    context = NEW(Foundation::getAllocator(), GlesContext);
#elif defined(PLATFORM_DESKTOP)
    context = NEW(Foundation::getAllocator(), VulkanContext);
#endif
    context->create();
    GL_CALL(glEnable(GL_BLEND));
    GL_CALL(glDisable(GL_STENCIL_TEST));
    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    // glBlendEquation(GL_FUNC_ADD);
    // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    BIRD_LOG("Vulkan VERSION {}", glGetString(GL_VERSION));
#if defined(PLATFORM_LINUX) || defined(PLATFORM_WINDOWS)
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gpuErrorCallback, nullptr);
#endif
    GL_CALL(glGenVertexArrays(1, &m_uselessVao));
    GL_CALL(glBindVertexArray(m_uselessVao));
}

RendererVulkan::~RendererVulkan() {
    GL_CALL(glDeleteVertexArrays(1, &m_uselessVao));
    DELETE(Foundation::getAllocator(), context);
    s_instance = nullptr;
}

RendererType RendererVulkan::getRendererType() const {
#ifdef PLATFORM_IOS
    return RendererType::VulkanES;
#elif defined(PLATFORM_DESKTOP)
    return RendererType::Vulkan;
#endif
}

void RendererVulkan::flip() {
    context->flip();
}

void RendererVulkan::createFrameBuffer(
    FrameBufferHandle handle, FrameBufferSpecification specification
) {
    frameBuffers[handle.id].create(specification);
}

void RendererVulkan::deleteFrameBuffer(FrameBufferHandle handle) {
    frameBuffers[handle.id].terminate();
}

void RendererVulkan::createProgram(ProgramHandle handle, ProgramCreate create) {
    shaders[handle.id].create(create);
}

void RendererVulkan::deleteShader(ProgramHandle handle) {
    shaders[handle.id].terminate();
}

void RendererVulkan::createTexture(TextureHandle handle, TextureCreate create) {
    textures[handle.id].create(create);
}

void RendererVulkan::resizeTexture(TextureHandle handle, uint32_t width, uint32_t height) {
    textures[handle.id].resize(width, height);
}

void RendererVulkan::deleteTexture(TextureHandle handle) {
    textures[handle.id].terminate();
}

void RendererVulkan::createIndexBuffer(
    IndexBufferHandle handle,
    Foundation::Memory indices,
    BufferElementType elementType,
    size_t count
) {
    indexBuffers[handle.id].create(indices.data, elementType, count, false);
    indices.release();
}

void RendererVulkan::createDynamicIndexBuffer(
    IndexBufferHandle handle,
    Foundation::Memory indices,
    BufferElementType elementType,
    size_t count
) {
    indexBuffers[handle.id].create(indices.data, elementType, count, true);
    indices.release();
}

void RendererVulkan::updateDynamicIndexBuffer(
    IndexBufferHandle handle, Foundation::Memory indices, size_t count
) {
    indexBuffers[handle.id].update(indices.data, count);
    indices.release();
}

void RendererVulkan::deleteIndexBuffer(IndexBufferHandle handle) {
    indexBuffers[handle.id].terminate();
}

void RendererVulkan::createVertexBuffer(
    VertexBufferHandle handle,
    Foundation::Memory data,
    uint32_t size,
    VertexLayoutHandle layoutHandle
) {
    vertexBuffers[handle.id].create(data.data, size, false);
    vertexBuffers[handle.id].setLayoutHandle(layoutHandle);
    data.release();
}

void RendererVulkan::createDynamicVertexBuffer(
    VertexBufferHandle handle,
    Foundation::Memory data,
    uint32_t size,
    VertexLayoutHandle layoutHandle
) {
    vertexBuffers[handle.id].create(data.data, size, true);
    vertexBuffers[handle.id].setLayoutHandle(layoutHandle);
    data.release();
}

void RendererVulkan::updateDynamicVertexBuffer(
    VertexBufferHandle handle, Foundation::Memory data, uint32_t size
) {
    vertexBuffers[handle.id].update(data.data, size);
    data.release();
}

void RendererVulkan::deleteVertexBuffer(VertexBufferHandle handle) {
    vertexBuffers[handle.id].terminate();
}

void RendererVulkan::createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) {
    vertexLayouts[handle.id] = layout;
}

void RendererVulkan::deleteVertexLayout(VertexLayoutHandle handle) {}

void RendererVulkan::setUniform(const Uniform &uniform) {
    shaders[uniform.handle.id].bind();
    switch (uniform.type) {
        case UniformType::Sampler:
            shaders[uniform.handle.id].setUniformInt(
                uniform.name, static_cast<int *>(uniform.data), uniform.count
            );
            return;
        case UniformType::Mat3:
            shaders[uniform.handle.id].setUniformMat3(
                uniform.name, static_cast<float *>(uniform.data), uniform.count
            );
            return;
        case UniformType::Mat4:
            shaders[uniform.handle.id].setUniformMat4(
                uniform.name, static_cast<float *>(uniform.data), uniform.count
            );
            return;
        case UniformType::Vec4:
            shaders[uniform.handle.id].setUniformVec4(
                uniform.name, static_cast<float *>(uniform.data), uniform.count
            );
            return;
        case UniformType::Count:
            return;
    }
    LOG_ERROR("UNIFORM TYPE IS UNDEFINED");

    LOG_ERROR("UNIFORM TYPE IS UNDEFINED");
}

void RendererVulkan::setTexture(TextureHandle handle, uint32_t slot) {
    textures[handle.id].bind(slot);
}

void RendererVulkan::submit(Frame *frame, View *views) {
    BIRD_LOG("FRAME SUBMITTED. DRAW CALLS: {}", frame->getDrawCallsCount());
    if (frame->m_transientVbSize > 0) {
        vertexBuffers[frame->m_transientVb.handle.id].update(
            frame->m_transientVb.data, frame->m_transientVbSize
        );
    }
    if (frame->m_transientIbSize > 0) {
        indexBuffers[frame->m_transientIb.handle.id].update(
            frame->m_transientIb.data, frame->m_transientIbSize / 2
        );
    }
    if (!frame->getDrawCallsCount()) {
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GL_CALL(glClearColor(0, 0, 0, 1));
        return;
    }
    ViewId viewId = -1;
    for (int i = 0; i < frame->getDrawCallsCount(); i++) {
        RenderDraw &draw = frame->getDrawCalls()[i];
        if (!draw.m_isSubmitted) {
            continue;
        }
        if (draw.m_viewId != viewId) {
            viewId = draw.m_viewId;
            viewChanged(views[viewId]);
        }
        submit(&draw);
    }
}

void RendererVulkan::viewChanged(View &view) {
    if (view.m_frameBuffer.isValid()) {
        frameBuffers[view.m_frameBuffer.id].bind();
    } else {
        GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, context->getDefaultFrameBufferId()));
    }
    if (!view.m_viewport.isZero()) {
        GL_CALL(glViewport(
            view.m_viewport.origin.x,
            view.m_viewport.origin.y,
            view.m_viewport.size.width,
            view.m_viewport.size.height
        ));
    }
    uint32_t rgba = view.m_clearColor;
    uint8_t r = rgba >> 24;
    uint8_t g = rgba >> 16;
    uint8_t b = rgba >> 8;
    uint8_t a = rgba >> 0;
    GL_CALL(glClearColor((r) / 255.f, (g) / 255.f, (b) / 255.f, (a) / 255.f));
    //    GL_CALL(glClearColor(1., 0., 0., 1.));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RendererVulkan::submit(RenderDraw *draw) {
    // TODO: Capture time
    shaders[draw->m_shader.id].bind();
    for (size_t u = 0; u < draw->m_uniformsCount; u++) {
        Uniform &uniform = draw->m_uniformBuffer[u];
        setUniform(uniform);
    }
    for (size_t t = 0; t < draw->m_textureBindingsCount; t++) {
        TextureBinding &textureBinding = draw->m_textureBindings[t];
        setTexture(textureBinding.m_handle, textureBinding.m_slot);
    }
    if (draw->m_state & BIRD_STATE_CULL_FACE) {
        GL_CALL(glEnable(GL_CULL_FACE));
    } else {
        GL_CALL(glDisable(GL_CULL_FACE));
    }
    if (draw->m_state & BIRD_STATE_DEPTH_TEST) {
        GL_CALL(glEnable(GL_DEPTH_TEST));
    } else {
        GL_CALL(glDisable(GL_DEPTH_TEST));
    }
    if (!draw->m_scissorRect.isZero()) {
        GL_CALL(glEnable(GL_SCISSOR_TEST));
        GL_CALL(glScissor(
            (int)draw->m_scissorRect.origin.x,
            (int)draw->m_scissorRect.origin.y,
            (int)draw->m_scissorRect.size.width,
            (int)draw->m_scissorRect.size.height
        ));
    } else {
        GL_CALL(glDisable(GL_SCISSOR_TEST));
    }
    vertexBuffers[draw->m_vertexBuffer.id].bind();
    VertexLayoutHandle layoutHandle =
        draw->m_vertexLayout.id != BIRD_INVALID_HANDLE
            ? draw->m_vertexLayout
            : vertexBuffers[draw->m_vertexBuffer.id].getLayoutHandle();
    NEST_ASSERT(layoutHandle.id != BIRD_INVALID_HANDLE, "Invalid handle");
    VertexBufferLayoutData &layout = vertexLayouts[layoutHandle.id];
    GL_CALL(glBindVertexArray(m_uselessVao));
    shaders[draw->m_shader.id].bindAttributes(layout, draw->m_verticesOffset);
    indexBuffers[draw->m_indexBuffer.id].bind();
    GL_CALL(glDrawElements(
        GL_TRIANGLES,
        draw->m_numIndices,
        indexBuffers[draw->m_indexBuffer.id].getElementType(),
        (void *)draw->m_indicesOffset
    ));
    indexBuffers[draw->m_indexBuffer.id].unbind();
    shaders[draw->m_shader.id].unbind();
    vertexBuffers[draw->m_vertexBuffer.id].unbind();
    GL_CALL(glDisable(GL_SCISSOR_TEST));
    GL_CALL(glBindVertexArray(0));
}

} // namespace Bird
