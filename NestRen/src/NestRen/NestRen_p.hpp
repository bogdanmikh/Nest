#pragma once

#include "Encoder/RendererCommand.hpp"
#include "Encoder/Frame.hpp"
#include "Encoder/View.hpp"
#include "Encoder/RenderDraw.hpp"
#include "NestRen/NestRen.hpp"
#include "HandleAllocator.hpp"
#include "NestRen/NestRenStates.hpp"
#include "NestRen/Config.hpp"
#include "Platform/RendererImpl/OpenGL/RendererOpenGL.hpp"
#include "RendererI.hpp"

#include <Foundation/PlatformDetection.hpp>
#include <Foundation/Logger.hpp>

namespace NestRen {

struct Context {
    // Должно быть вызвано из главного потока и не из цикла обновления.
    Context()
        : m_renderer(nullptr)
        , m_frameNumber(0)
        , m_frameBuffersHandleAlloc(MAX_FRAME_BUFFERS)
        , m_shadersHandleAlloc(MAX_SHADERS)
        , m_texturesHandleAlloc(MAX_TEXTURES)
        , m_vertexLayoutsHandleAlloc(MAX_BUFFER_LAYOUTS)
        , m_vertexBuffersHandleAlloc(MAX_VERTEX_BUFFERS)
        , m_indexBuffersHandleAlloc(MAX_INDEX_BUFFERS)
        , m_preCommandQueue(300000)
        , m_postCommandQueue(300000) {

        m_render = &m_frame[0];
        m_submit = &m_frame[1];

        Foundation::CommandBuffer::Command cmd(RendererCommandType::RendererInit);
        m_preCommandQueue.write(cmd);
        m_submit->m_transientVb = createTransientVertexBuffer(TRANSIENT_VERTEX_BUFFER_SIZE);
        m_submit->m_transientIb = createTransientIndexBuffer(TRANSIENT_INDEX_BUFFER_SIZE);
        frame();
        m_submit->m_transientVb = createTransientVertexBuffer(TRANSIENT_VERTEX_BUFFER_SIZE);
        m_submit->m_transientIb = createTransientIndexBuffer(TRANSIENT_INDEX_BUFFER_SIZE);
        frame();
    }

    // Должно быть вызвано из главного потока и не из цикла обновления.
    ~Context() {
        destroyTransientVertexBuffer(m_submit->m_transientVb);
        destroyTransientIndexBuffer(m_submit->m_transientIb);
        frame();
        destroyTransientVertexBuffer(m_submit->m_transientVb);
        destroyTransientIndexBuffer(m_submit->m_transientIb);
        frame();
        Foundation::CommandBuffer::Command cmd(RendererCommandType::RendererShutdown);
        m_postCommandQueue.write(cmd);
    }

    void rendererExecuteCommands(Foundation::CommandBuffer &commandBuffer) {
        Foundation::CommandBuffer::Command *command;
        while ((command = commandBuffer.read()) != nullptr) {
            CMDBUF_LOG("COMMAND: {}", command->type);
            switch (command->type) {
                case RendererCommandType::RendererInit: {
                    break;
                }
                case RendererCommandType::RendererShutdown: {
                    CMDBUF_LOG("RENDERER SHUTDOWN COMMAND");
                    DELETE(Foundation::getAllocator(), m_renderer);
                    m_renderer = nullptr;
                    return;
                }
                case RendererCommandType::CreateFrameBuffer: {
                    CMDBUF_LOG("CREATE FRAME BUFFER COMMAND");
                    const CreateFrameBufferCommand *cmd =
                        static_cast<const CreateFrameBufferCommand *>(command);
                    m_renderer->createFrameBuffer(cmd->handle, cmd->spec);
                    break;
                }
                case RendererCommandType::DestroyFrameBuffer: {
                    CMDBUF_LOG("DESTROY FRAME BUFFER COMMAND");
                    const DeleteFrameBufferCommand *cmd =
                        static_cast<const DeleteFrameBufferCommand *>(command);
                    m_renderer->deleteFrameBuffer(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateProgram: {
                    CMDBUF_LOG("CREATE SHADER COMMAND");
                    const CreateProgramCommand *cmd =
                        static_cast<const CreateProgramCommand *>(command);
                    m_renderer->createProgram(cmd->handle, cmd->create);
                    break;
                }
                case RendererCommandType::DestroyProgram: {
                    CMDBUF_LOG("DESTROY SHADER COMMAND");
                    const DeleteProgramCommand *cmd =
                        static_cast<const DeleteProgramCommand *>(command);
                    m_renderer->deleteShader(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateTexture: {
                    CMDBUF_LOG("CREATE TEXTURE COMMAND");
                    const CreateTextureCommand *cmd =
                        static_cast<const CreateTextureCommand *>(command);
                    m_renderer->createTexture(cmd->handle, cmd->create);
                    break;
                }
                case RendererCommandType::ResizeTexture: {
                    CMDBUF_LOG("RESIZE TEXTURE COMMAND");
                    const ResizeTextureCommand *cmd =
                        static_cast<const ResizeTextureCommand *>(command);
                    m_renderer->resizeTexture(cmd->handle, cmd->width, cmd->height);
                    break;
                }
                case RendererCommandType::DestroyTexture: {
                    CMDBUF_LOG("DESTROY TEXTURE COMMAND");
                    const DeleteTextureCommand *cmd =
                        static_cast<const DeleteTextureCommand *>(command);
                    m_renderer->deleteTexture(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateIndexBuffer: {
                    CMDBUF_LOG("CREATE INDEX BUFFER COMMAND");
                    const CreateIndexBufferCommand *cmd =
                        static_cast<const CreateIndexBufferCommand *>(command);
                    m_renderer->createIndexBuffer(
                        cmd->handle, cmd->indices, cmd->elementType, cmd->count
                    );
                    break;
                }
                case RendererCommandType::CreateDynamicIndexBuffer: {
                    CMDBUF_LOG("CREATE DYNAMIC INDEX BUFFER COMMAND");
                    const CreateDynamicIndexBufferCommand *cmd =
                        static_cast<const CreateDynamicIndexBufferCommand *>(command);
                    m_renderer->createDynamicIndexBuffer(
                        cmd->handle, cmd->indices, cmd->elementType, cmd->count
                    );
                    break;
                }
                case RendererCommandType::UpdateDynamicIndexBuffer: {
                    CMDBUF_LOG("UPDATE DYNAMIC INDEX BUFFER COMMAND");
                    const UpdateDynamicIndexBufferCommand *cmd =
                        static_cast<const UpdateDynamicIndexBufferCommand *>(command);
                    m_renderer->updateDynamicIndexBuffer(cmd->handle, cmd->indices, cmd->count);
                    break;
                }
                case RendererCommandType::DestroyIndexBuffer: {
                    CMDBUF_LOG("DESTROY INDEX BUFFER COMMAND");
                    const DeleteIndexBufferCommand *cmd =
                        static_cast<const DeleteIndexBufferCommand *>(command);
                    m_renderer->deleteIndexBuffer(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateVertexBuffer: {
                    CMDBUF_LOG("CREATE VERTEX BUFFER COMMAND");
                    const CreateVertexBufferCommand *cmd =
                        static_cast<const CreateVertexBufferCommand *>(command);
                    m_renderer->createVertexBuffer(
                        cmd->handle, cmd->data, cmd->size, cmd->layoutHandle
                    );
                    break;
                }
                case RendererCommandType::CreateDynamicVertexBuffer: {
                    CMDBUF_LOG("CREATE DYNAMIC VERTEX BUFFER COMMAND");
                    const CreateDynamicVertexBufferCommand *cmd =
                        static_cast<const CreateDynamicVertexBufferCommand *>(command);
                    m_renderer->createDynamicVertexBuffer(
                        cmd->handle, cmd->data, cmd->size, cmd->layoutHandle
                    );
                    break;
                }
                case RendererCommandType::UpdateDynamicVertexBuffer: {
                    CMDBUF_LOG("UPDATE DYNAMIC VERTEX BUFFER COMMAND");
                    const UpdateDynamicVertexBufferCommand *cmd =
                        static_cast<const UpdateDynamicVertexBufferCommand *>(command);
                    m_renderer->updateDynamicVertexBuffer(cmd->handle, cmd->data, cmd->size);
                    break;
                }
                case RendererCommandType::DestroyVertexBuffer: {
                    CMDBUF_LOG("DESTROY VERTEX BUFFER COMMAND");
                    const DeleteVertexBufferCommand *cmd =
                        static_cast<const DeleteVertexBufferCommand *>(command);
                    m_renderer->deleteVertexBuffer(cmd->handle);
                    break;
                }
                case RendererCommandType::CreateVertexLayout: {
                    CMDBUF_LOG("CREATE VERTEX LAYOUT COMMAND");
                    const CreateVertexLayoutCommand *cmd =
                        static_cast<const CreateVertexLayoutCommand *>(command);
                    m_renderer->createVertexLayout(cmd->handle, cmd->data);
                    break;
                }
                case RendererCommandType::DestroyVertexLayout: {
                    CMDBUF_LOG("DESTROY VERTEX LAYOUT COMMAND");
                    const DeleteVertexLayoutCommand *cmd =
                        static_cast<const DeleteVertexLayoutCommand *>(command);
                    m_renderer->deleteVertexLayout(cmd->handle);
                    break;
                }
                default: {
                    LOG_CRITICAL("UNKNOWN COMMAND");
                }
            }
        }
    }

    void checkIfHasInitCommand() {
        Foundation::CommandBuffer::Command *command = m_preCommandQueue.read();
        if (command != nullptr) {
            CMDBUF_LOG("RENDERER INIT COMMAND");
            NEST_ASSERT(
                command->type == RendererCommandType::RendererInit,
                "First command should be RendererInit"
            );
            m_renderer = NEW(Foundation::getAllocator(), RendererOpenGL);
            NESTREN_LOG("RENDERER CREATED");
        }
    }

    bool renderFrame() {
        LOG_INFO("RENDER FRAME BEGIN");
        m_preCommandQueue.finishWriting();
        m_postCommandQueue.finishWriting();
        if (m_renderer == nullptr) {
            checkIfHasInitCommand();
        }
        if (m_renderer == nullptr) {
            m_preCommandQueue.reset();
            m_postCommandQueue.reset();
            return true;
        }
        rendererExecuteCommands(m_preCommandQueue);
        if (m_render->getDrawCallsCount() != 0) {
            m_renderer->flip();
            m_renderer->submit(m_render, m_views);
        }
        rendererExecuteCommands(m_postCommandQueue);
        m_preCommandQueue.reset();
        m_postCommandQueue.reset();
        LOG_INFO("RENDER FRAME END");
        return m_renderer != nullptr;
    }

    void freeAllHandles(Frame *frame) {
        frame->free(&m_frameBuffersHandleAlloc);
        frame->free(&m_shadersHandleAlloc);
        frame->free(&m_texturesHandleAlloc);
        frame->free(&m_vertexLayoutsHandleAlloc);
        frame->free(&m_vertexBuffersHandleAlloc);
        frame->free(&m_indexBuffersHandleAlloc);
    }

    FrameBufferHandle createFrameBuffer(FrameBufferSpecification specification) {
        FrameBufferHandle handle = m_frameBuffersHandleAlloc.alloc();
        CreateFrameBufferCommand cmd(handle, specification);
        m_preCommandQueue.write(cmd);
        return handle;
    }

    void deleteFrameBuffer(FrameBufferHandle handle) {
        m_submit->queueFree(handle);
        DeleteFrameBufferCommand cmd(handle);
        m_postCommandQueue.write(cmd);
    }

    ProgramHandle createProgram(ProgramCreate create) {
        ProgramHandle handle = m_shadersHandleAlloc.alloc();
        CreateProgramCommand cmd(handle, create);
        m_preCommandQueue.write(cmd);
        return handle;
    }

    void deleteProgram(ProgramHandle handle) {
        m_submit->queueFree(handle);
        DeleteProgramCommand cmd(handle);
        m_postCommandQueue.write(cmd);
    }

    TextureHandle createTexture(TextureCreate create) {
        TextureHandle handle = m_texturesHandleAlloc.alloc();
        CreateTextureCommand cmd(handle, create);
        m_preCommandQueue.write(cmd);
        return handle;
    }

    void resizeTexture(TextureHandle handle, uint32_t width, uint32_t height) {
        ResizeTextureCommand cmd(handle, width, height);
        m_preCommandQueue.write(cmd);
    }

    void deleteTexture(TextureHandle handle) {
        m_submit->queueFree(handle);
        DeleteTextureCommand cmd(handle);
        m_postCommandQueue.write(cmd);
    }

    IndexBufferHandle
    createIndexBuffer(Foundation::Memory indices, BufferElementType elementType, size_t count) {
        IndexBufferHandle handle = m_indexBuffersHandleAlloc.alloc();
        CreateIndexBufferCommand cmd(handle, indices, elementType, count);
        m_preCommandQueue.write(cmd);
        return handle;
    }

    IndexBufferHandle createDynamicIndexBuffer(
        Foundation::Memory indices, BufferElementType elementType, size_t count
    ) {
        IndexBufferHandle handle = m_indexBuffersHandleAlloc.alloc();
        CreateDynamicIndexBufferCommand cmd(handle, indices, elementType, count);
        m_preCommandQueue.write(cmd);
        return handle;
    }

    void
    updateDynamicIndexBuffer(IndexBufferHandle handle, Foundation::Memory indices, size_t count) {
        UpdateDynamicIndexBufferCommand cmd(handle, indices, count);
        m_preCommandQueue.write(cmd);
    }

    void deleteIndexBuffer(IndexBufferHandle handle) {
        m_submit->queueFree(handle);
        DeleteIndexBufferCommand cmd(handle);
        m_postCommandQueue.write(cmd);
    }

    VertexBufferHandle
    createVertexBuffer(Foundation::Memory data, uint32_t size, VertexLayoutHandle layoutHandle) {
        VertexBufferHandle handle = m_vertexBuffersHandleAlloc.alloc();
        CreateVertexBufferCommand cmd(handle, data, size, layoutHandle);
        m_preCommandQueue.write(cmd);
        return handle;
    }

    VertexBufferHandle createDynamicVertexBuffer(
        Foundation::Memory data,
        uint32_t size,
        VertexLayoutHandle layoutHandle = NESTREN_INVALID_HANDLE
    ) {
        VertexBufferHandle handle = m_vertexBuffersHandleAlloc.alloc();
        CreateDynamicVertexBufferCommand cmd(handle, data, size, layoutHandle);
        m_preCommandQueue.write(cmd);
        return handle;
    }

    void
    updateDynamicVertexBuffer(VertexBufferHandle handle, Foundation::Memory data, uint32_t size) {
        UpdateDynamicVertexBufferCommand cmd(handle, data, size);
        m_preCommandQueue.write(cmd);
    }

    void deleteVertexBuffer(VertexBufferHandle handle) {
        m_submit->queueFree(handle);
        DeleteVertexBufferCommand cmd(handle);
        m_postCommandQueue.write(cmd);
    }

    VertexLayoutHandle createVertexLayout(VertexBufferLayoutData data) {
        VertexLayoutHandle handle = m_vertexLayoutsHandleAlloc.alloc();
        CreateVertexLayoutCommand cmd(handle, data);
        m_preCommandQueue.write(cmd);
        return handle;
    }

    void deleteVertexLayout(VertexLayoutHandle handle) {
        m_submit->queueFree(handle);
        DeleteVertexLayoutCommand cmd(handle);
        m_postCommandQueue.write(cmd);
    }

    void destroyTransientVertexBuffer(TransientVertexBuffer &tvb) {
        deleteVertexBuffer(tvb.handle);
        FREE(Foundation::getAllocator(), tvb.data);
    }

    void destroyTransientIndexBuffer(TransientIndexBuffer &tib) {
        deleteIndexBuffer(tib.handle);
        FREE(Foundation::getAllocator(), tib.data);
    }

    TransientIndexBuffer createTransientIndexBuffer(uint32_t size) {
        TransientIndexBuffer tib;
        tib.data = (uint8_t *)NEW_ARRAY(Foundation::getAllocator(), uint8_t, size);
        tib.size = size;
        tib.startIndex = 0;
        tib.handle = createDynamicIndexBuffer(nullptr, BufferElementType::UnsignedShort, size / 2);
        tib.elementType = BufferElementType::UnsignedShort;
        return tib;
    }

    TransientVertexBuffer createTransientVertexBuffer(uint32_t size) {
        TransientVertexBuffer tvb;
        tvb.data = (uint8_t *)NEW_ARRAY(Foundation::getAllocator(), uint8_t, size);
        tvb.size = size;
        tvb.startVertex = 0;
        tvb.handle = createDynamicVertexBuffer(nullptr, size);
        return tvb;
    }

    void allocTransientVertexBuffer(TransientVertexBuffer *buffer, uint32_t size) {
        uint32_t transientVBOffset = m_submit->m_transientVbSize;
        buffer->data = &m_submit->m_transientVb.data[transientVBOffset];
        m_submit->m_transientVbSize += size;
        NEST_ASSERT(
            m_submit->m_transientVbSize < TRANSIENT_VERTEX_BUFFER_SIZE, "INCREASE DEFAULT VB SIZE"
        );
        buffer->size = size;
        buffer->startVertex = transientVBOffset;
        buffer->handle = m_submit->m_transientVb.handle;
    }

    void allocTransientIndexBuffer(
        TransientIndexBuffer *buffer, uint32_t count, BufferElementType elementType
    ) {
        uint32_t transientIBOffset = m_submit->m_transientIbSize;
        uint32_t elementSize = VertexBufferElement::getSizeOfType(elementType);
        uint32_t size = count * elementSize;
        buffer->data = &m_submit->m_transientIb.data[transientIBOffset];
        m_submit->m_transientIbSize += size;
        NEST_ASSERT(
            m_submit->m_transientIbSize < TRANSIENT_INDEX_BUFFER_SIZE, "INCREASE DEFAULT IB SIZE"
        );
        buffer->size = size;
        buffer->startIndex = transientIBOffset;
        buffer->elementType = elementType;
        buffer->handle = m_submit->m_transientIb.handle;
    }

    void setState(uint32_t state) {
        m_submit->setState(state);
    }

    void setScissorRect(Rect rect) {
        m_submit->setScissorRect(rect);
    }

    void setVertexBuffer(VertexBufferHandle handle, intptr_t offset) {
        m_submit->setVertexBuffer(handle, offset);
    }

    void setIndexBuffer(IndexBufferHandle handle, intptr_t offset, size_t count) {
        m_submit->setIndexBuffer(handle, offset, count);
    }

    void setShader(ProgramHandle handle) {
        m_submit->setShader(handle);
    }

    void setUniform(ProgramHandle handle, const char *name, void *value, UniformType type, int count) {
        m_submit->setUniform(handle, name, value, type, count);
    }

    void setTexture(TextureHandle textureHandle, uint32_t slot) {
        m_submit->setTexture(textureHandle, slot);
    }

    void setVertexLayout(VertexLayoutHandle handle) {
        m_submit->setVertexLayout(handle);
    }

    void submit(ViewId id) {
        m_submit->submitCurrentDrawCall(id);
        m_submit->beginDrawCall();
    }

    void swap() {
        std::swap(m_render, m_submit);
    }

    uint32_t frame() {
        freeAllHandles(m_submit);
        swap();
        m_submit->reset();
        return m_frameNumber++;
    }

    void setViewport(ViewId id, Rect rect) {
        m_views[id].m_viewport = rect;
    }

    void setViewClear(ViewId id, uint32_t color) {
        m_views[id].m_clearColor = color;
    }

    void setViewFrameBuffer(ViewId id, FrameBufferHandle frameBuffer) {
        m_views[id].m_frameBuffer = frameBuffer;
    }

//    Foundation::Thread m_thread;
private:
    RendererI *m_renderer;
    Frame m_frame[2];
    Frame *m_render;
    Frame *m_submit;
    View m_views[MAX_VIEWS];
    uint32_t m_frameNumber;

    // TODO: - Add reusable buffers:
    // DynamicIndexBuffer m_dynamicIndexBuffers[MAX_DYNAMIC_INDEX_BUFFERS];
    // DynamicVertexBuffer m_dynamicVertexBuffers[MAX_DYNAMIC_VERTEX_BUFFERS];

    HandleAllocator<FrameBufferHandle> m_frameBuffersHandleAlloc;
    HandleAllocator<ProgramHandle> m_shadersHandleAlloc;
    HandleAllocator<TextureHandle> m_texturesHandleAlloc;
    HandleAllocator<VertexLayoutHandle> m_vertexLayoutsHandleAlloc;
    HandleAllocator<VertexBufferHandle> m_vertexBuffersHandleAlloc;
    HandleAllocator<IndexBufferHandle> m_indexBuffersHandleAlloc;
    Foundation::CommandBuffer m_preCommandQueue;
    Foundation::CommandBuffer m_postCommandQueue;

public:
//    Foundation::Semaphore m_rendererSemaphore;
};

} // namespace NestRen
