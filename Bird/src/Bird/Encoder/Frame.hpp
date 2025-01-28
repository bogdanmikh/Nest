//
// Created by Admin on 14.03.2022.
//

#pragma once

#include "Bird/Base.hpp"
#include "Bird/HandleAllocator.hpp"
#include "Bird/VertexBufferLayoutData.hpp"
#include "Bird/Encoder/RenderDraw.hpp"

#include <Foundation/CommandBuffer.hpp>

namespace Bird {

/// Класс используемый для хранения данных о кадре.
/// Тут хранятся буферы, которые существуют только один кадр и данные о графических вызовах
class Frame {
public:
    Frame();
    void beginDrawCall();
    void setState(uint32_t state);
    void setIndexBuffer(IndexBufferHandle handle, intptr_t offset, uint32_t count);
    void setVertexBuffer(VertexBufferHandle handle, intptr_t offset);
    void setShader(ProgramHandle handle);
    void setVertexLayout(VertexLayoutHandle handle);
    void setScissorRect(Rect rect);
    void
    setUniform(ProgramHandle handle, const char *name, void *value, UniformType type, int count);
    void setTexture(TextureHandle textureHandle, uint32_t slot);
    void submitCurrentDrawCall(ViewId id);
    void queueFree(FrameBufferHandle handle);
    void queueFree(ProgramHandle handle);
    void queueFree(TextureHandle handle);
    void queueFree(VertexBufferHandle handle);
    void queueFree(VertexLayoutHandle handle);
    void queueFree(IndexBufferHandle handle);
    void free(HandleAllocator<FrameBufferHandle> *allocator);
    void free(HandleAllocator<ProgramHandle> *allocator);
    void free(HandleAllocator<TextureHandle> *allocator);
    void free(HandleAllocator<VertexBufferHandle> *allocator);
    void free(HandleAllocator<VertexLayoutHandle> *allocator);
    void free(HandleAllocator<IndexBufferHandle> *allocator);
    uint32_t getDrawCallsCount();
    RenderDraw *getDrawCalls();
    void reset();
    Foundation::CommandBuffer &getPreCommandQueue();
    Foundation::CommandBuffer &getPostCommandQueue();

    TransientIndexBuffer m_transientIb;
    TransientVertexBuffer m_transientVb;
    uint32_t m_transientVbSize;
    uint32_t m_transientIbSize;

private:
    int m_drawCallsCount;
    RenderDraw m_drawCalls[MAX_DRAW_CALLS];
    FreeHandleQueue<FrameBufferHandle, MAX_FRAME_BUFFERS> m_frameBuffersFreeHandle;
    FreeHandleQueue<ProgramHandle, MAX_SHADERS> m_shadersFreeHandle;
    FreeHandleQueue<TextureHandle, MAX_TEXTURES> m_texturesFreeHandle;
    FreeHandleQueue<VertexLayoutHandle, MAX_BUFFER_LAYOUTS> m_vertexLayoutsFreeHandle;
    FreeHandleQueue<VertexBufferHandle, MAX_VERTEX_BUFFERS> m_vertexBuffersFreeHandle;
    FreeHandleQueue<IndexBufferHandle, MAX_INDEX_BUFFERS> m_indexBuffersFreeHandle;
    Foundation::CommandBuffer m_preCommandQueue;
    Foundation::CommandBuffer m_postCommandQueue;
};

} // namespace Bird
