//
// Created by Admin on 14.03.2022.
//

#include "Frame.hpp"

#include <Foundation/Allocator.hpp>

namespace Miren {

Frame::Frame()
    : m_drawCallsCount(0) {}

void Frame::beginDrawCall() {
    m_drawCallsCount++;
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.reset();
}

void Frame::setState(uint32_t state) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_state = state;
}

void Frame::setIndexBuffer(IndexBufferHandle handle, intptr_t offset, uint32_t count) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_indexBuffer = handle;
    draw.m_indicesOffset = offset;
    draw.m_numIndices = count;
}

void Frame::setVertexBuffer(VertexBufferHandle handle, intptr_t offset) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_vertexBuffer = handle;
    draw.m_verticesOffset = offset;
}

void Frame::setVertexLayout(VertexLayoutHandle handle) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_vertexLayout = handle;
}

void Frame::setShader(ProgramHandle handle) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_shader = handle;
}

void Frame::setUniform(ProgramHandle handle, const char *name, void *value, UniformDataType type) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.addUniform(handle, name, value, type);
}

void Frame::setTexture(TextureHandle textureHandle, uint32_t slot) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.setTexture(textureHandle, slot);
}

void Frame::setScissorRect(Rect rect) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_scissorRect = rect;
}

void Frame::submitCurrentDrawCall(ViewId id) {
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.m_viewId = id;
    draw.m_isSubmitted = true;
}

uint32_t Frame::getDrawCallsCount() {
    return m_drawCallsCount;
}

RenderDraw *Frame::getDrawCalls() {
    return m_drawCalls;
}

void Frame::queueFree(FrameBufferHandle handle) {
    m_frameBuffersFreeHandle.queue(handle);
}

void Frame::queueFree(ProgramHandle handle) {
    m_shadersFreeHandle.queue(handle);
}

void Frame::queueFree(TextureHandle handle) {
    m_texturesFreeHandle.queue(handle);
}

void Frame::queueFree(VertexBufferHandle handle) {
    m_vertexBuffersFreeHandle.queue(handle);
}

void Frame::queueFree(VertexLayoutHandle handle) {
    m_vertexLayoutsFreeHandle.queue(handle);
}

void Frame::queueFree(IndexBufferHandle handle) {
    m_indexBuffersFreeHandle.queue(handle);
}

void Frame::free(HandleAllocator<FrameBufferHandle> *allocator) {
    for (int i = 0; i < m_frameBuffersFreeHandle.getNumQueued(); i++) {
        allocator->free(m_frameBuffersFreeHandle.get(i));
    }
}

void Frame::free(HandleAllocator<ProgramHandle> *allocator) {
    for (int i = 0; i < m_shadersFreeHandle.getNumQueued(); i++) {
        allocator->free(m_shadersFreeHandle.get(i));
    }
}

void Frame::free(HandleAllocator<TextureHandle> *allocator) {
    for (int i = 0; i < m_texturesFreeHandle.getNumQueued(); i++) {
        allocator->free(m_texturesFreeHandle.get(i));
    }
}

void Frame::free(HandleAllocator<VertexBufferHandle> *allocator) {
    for (int i = 0; i < m_vertexBuffersFreeHandle.getNumQueued(); i++) {
        allocator->free(m_vertexBuffersFreeHandle.get(i));
    }
}

void Frame::free(HandleAllocator<VertexLayoutHandle> *allocator) {
    for (int i = 0; i < m_vertexLayoutsFreeHandle.getNumQueued(); i++) {
        allocator->free(m_vertexLayoutsFreeHandle.get(i));
    }
}

void Frame::free(HandleAllocator<IndexBufferHandle> *allocator) {
    for (int i = 0; i < m_indexBuffersFreeHandle.getNumQueued(); i++) {
        allocator->free(m_indexBuffersFreeHandle.get(i));
    }
}

void Frame::reset() {
    m_drawCallsCount = 0;
    m_transientVbSize = 0;
    m_transientIbSize = 0;
    m_frameBuffersFreeHandle.reset();
    m_shadersFreeHandle.reset();
    m_texturesFreeHandle.reset();
    m_vertexLayoutsFreeHandle.reset();
    m_vertexBuffersFreeHandle.reset();
    m_indexBuffersFreeHandle.reset();
    RenderDraw &draw = m_drawCalls[m_drawCallsCount];
    draw.reset();
}

} // namespace Miren
