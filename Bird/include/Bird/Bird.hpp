//
// Created by Admin on 10.03.2022.
//

#pragma once

#include "Base.hpp"
#include "BirdStates.hpp"
#include "VertexBufferLayoutData.hpp"

namespace Bird {

void initialize();
void terminate();
// MARK: - Command buffer
FrameBufferHandle createFrameBuffer(FrameBufferSpecification specification);
uint32_t readFrameBuffer(
    FrameBufferHandle handle, int attachIndex, int x, int y, int width, int height, void *data
);
void deleteFrameBuffer(FrameBufferHandle handle);
ProgramHandle createProgram(ProgramCreate create);
void deleteProgram(ProgramHandle handle);
TextureHandle createTexture(TextureCreate create);
void resizeTexture(TextureHandle handle, uint32_t width, uint32_t height);
void deleteTexture(TextureHandle handle);
IndexBufferHandle
createIndexBuffer(Foundation::Memory indices, BufferElementType elementType, size_t count);
IndexBufferHandle
createDynamicIndexBuffer(Foundation::Memory indices, BufferElementType elementType, size_t count);
void updateDynamicIndexBuffer(IndexBufferHandle, Foundation::Memory indices, size_t count);
void deleteIndexBuffer(IndexBufferHandle handle);
// Only next frame vertex buffer
void allocTransientVertexBuffer(TransientVertexBuffer *buffer, uint32_t size);
// Only next frame index buffer
void allocTransientIndexBuffer(
    TransientIndexBuffer *buffer, uint32_t count, BufferElementType elementType
);
VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data);
VertexBufferHandle
createVertexBuffer(Foundation::Memory vertices, uint32_t size, VertexLayoutHandle layoutHandle);
VertexBufferHandle createDynamicVertexBuffer(
    Foundation::Memory, uint32_t size, VertexLayoutHandle layoutHandle = BIRD_INVALID_HANDLE
);
void updateDynamicVertexBuffer(VertexBufferHandle handle, Foundation::Memory data, uint32_t size);
void deleteVertexBuffer(VertexBufferHandle handle);
void deleteVertexLayout(VertexLayoutHandle handle);
uint32_t readTexture(TextureHandle handle, void *data);
// MARK: - Encoder setup
void setViewClear(ViewId id, uint32_t color);
void setViewClearAttachments(ViewId id, std::vector<Clear> clear);
void setViewport(ViewId id, Rect rect);
void setViewFrameBuffer(ViewId id, FrameBufferHandle frameBuffer);
void setState(uint32_t state);
void setScissorRect(Rect rect);
void setUniform(
    ProgramHandle handle,
    const char *name,
    void *value,
    UniformType type = UniformType::Sampler,
    int count = 1
);
void setVertexBuffer(VertexBufferHandle handle, intptr_t offset = 0);
void setIndexBuffer(IndexBufferHandle handle, intptr_t offset, size_t count);
void setVertexLayout(VertexLayoutHandle handle);
void setShader(ProgramHandle handle);
void setTexture(TextureHandle textureHandle, uint32_t slot);
/// Submit draw call
void submit(ViewId id);
// MARK: - Main functions
/// Process all requests to gpu (from rendering thread)
bool renderFrame();
/// Frame processing finished (from app thread). Wait for renderer to finish rendering frame.
/// Returns frame number
uint32_t frame();

} // namespace Bird
