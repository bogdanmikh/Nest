//
// Created by Admin on 10.03.2022.
//

#include "NestRen/NestRen_p.hpp"

namespace NestRen {

// MARK: - PUBLIC METHODS IMPL

static Context *s_context = nullptr;
//static const int CONTEXT_ALIGNMENT = 64;

void initialize() {
    NESTREN_LOG("NESTREN INIT BEGIN");
    NESTREN_LOG("ALLOCATING NESTREN CONTEXT, {} BYTES", sizeof(Context));
    NESTREN_LOG("FRAME DATA SIZE: {} BYTES", sizeof(Frame));
    s_context = NEW(Foundation::getAllocator(), Context);
    // Вызвано из главного потока: можно стартовать поток отрисовки.
    NESTREN_LOG("NESTREN INIT END");
}

void terminate() {
    NESTREN_LOG("NESTREN SHUTDOWN BEGIN");
    DELETE(Foundation::getAllocator(), s_context);
    s_context = nullptr;
    NESTREN_LOG("NESTREN SHUTDOWN END");
}

FrameBufferHandle createFrameBuffer(FrameBufferSpecification specification) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    return s_context->createFrameBuffer(specification);
}

void deleteFrameBuffer(FrameBufferHandle handle) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->deleteFrameBuffer(handle);
}

ProgramHandle createProgram(ProgramCreate create) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    return s_context->createProgram(create);
}

void deleteProgram(ProgramHandle handle) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->deleteProgram(handle);
}

TextureHandle createTexture(TextureCreate create) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    NEST_ASSERT_F(
        create.m_width > 0 && create.m_height > 0,
        "INVALID TEXTURE SIZE w: {}, h: {}",
        create.m_width,
        create.m_height
    );
    return s_context->createTexture(create);
}

void resizeTexture(TextureHandle handle, uint32_t width, uint32_t height) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    NEST_ASSERT_F(width > 0 && height > 0, "INVALID TEXTURE SIZE w: {}, h: {}", width, height);
    s_context->resizeTexture(handle, width, height);
}

void deleteTexture(TextureHandle handle) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->deleteTexture(handle);
}

IndexBufferHandle
createIndexBuffer(Foundation::Memory indices, BufferElementType elementType, size_t count) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    return s_context->createIndexBuffer(indices, elementType, count);
}

IndexBufferHandle
createDynamicIndexBuffer(Foundation::Memory indices, BufferElementType elementType, size_t count) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    return s_context->createDynamicIndexBuffer(indices, elementType, count);
}

void updateDynamicIndexBuffer(IndexBufferHandle handle, Foundation::Memory indices, size_t count) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->updateDynamicIndexBuffer(handle, indices, count);
}

void deleteIndexBuffer(IndexBufferHandle handle) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->deleteIndexBuffer(handle);
}

VertexBufferHandle
createVertexBuffer(Foundation::Memory data, uint32_t size, VertexLayoutHandle layoutHandle) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    return s_context->createVertexBuffer(data, size, layoutHandle);
}

VertexBufferHandle
createDynamicVertexBuffer(Foundation::Memory data, uint32_t size, VertexLayoutHandle layoutHandle) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    return s_context->createDynamicVertexBuffer(data, size, layoutHandle);
}

void updateDynamicVertexBuffer(VertexBufferHandle handle, Foundation::Memory data, uint32_t size) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->updateDynamicVertexBuffer(handle, data, size);
}

void deleteVertexBuffer(VertexBufferHandle handle) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->deleteVertexBuffer(handle);
}

VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    return s_context->createVertexLayout(data);
}

void deleteVertexLayout(VertexLayoutHandle handle) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->deleteVertexLayout(handle);
}

void allocTransientVertexBuffer(TransientVertexBuffer *buffer, uint32_t size) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->allocTransientVertexBuffer(buffer, size);
}

void allocTransientIndexBuffer(
    TransientIndexBuffer *buffer, uint32_t count, BufferElementType elementType
) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->allocTransientIndexBuffer(buffer, count, elementType);
}

bool renderFrame() {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    return s_context->renderFrame();
}

uint32_t frame() {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    return s_context->frame();
}

void setViewClear(ViewId id, uint32_t color) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->setViewClear(id, color);
}

void setViewport(ViewId id, Rect rect) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->setViewport(id, rect);
}

void setViewFrameBuffer(ViewId id, FrameBufferHandle frameBuffer) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->setViewFrameBuffer(id, frameBuffer);
}

void setState(uint32_t state) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->setState(state);
}

void setScissorRect(Rect rect) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->setScissorRect(rect);
}

void setVertexBuffer(VertexBufferHandle handle, intptr_t offset) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->setVertexBuffer(handle, offset);
}

void setIndexBuffer(IndexBufferHandle handle, intptr_t offset, size_t count) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->setIndexBuffer(handle, offset, count);
}

void setShader(ProgramHandle handle) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->setShader(handle);
}

void setUniform(ProgramHandle handle, const char *name, void *value, UniformType type, int count) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->setUniform(handle, name, value, type, count);
}

void setTexture(TextureHandle textureHandle, uint32_t slot) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->setTexture(textureHandle, slot);
}

void setVertexLayout(VertexLayoutHandle handle) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->setVertexLayout(handle);
}

void submit(ViewId id) {
    NEST_ASSERT(s_context != nullptr, "NESTREN NOT INITIALIZED");
    s_context->submit(id);
}

} // namespace NESTREN
