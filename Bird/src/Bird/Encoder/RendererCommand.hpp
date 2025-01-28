//
// Created by Admin on 05.03.2022.
//

#pragma once

#include "Bird/VertexBufferLayoutData.hpp"
#include "Bird/RendererI.hpp"

#include <Foundation/CommandBuffer.hpp>

namespace Bird {

enum RendererCommandType {
    RendererInit,
    RendererShutdown,
    CreateFrameBuffer,
    DestroyFrameBuffer,
    CreateProgram,
    DestroyProgram,
    CreateTexture,
    ResizeTexture,
    DestroyTexture,
    CreateIndexBuffer,
    CreateDynamicIndexBuffer,
    UpdateDynamicIndexBuffer,
    DestroyIndexBuffer,
    CreateVertexBuffer,
    CreateDynamicVertexBuffer,
    UpdateDynamicVertexBuffer,
    DestroyVertexBuffer,
    CreateVertexLayout,
    DestroyVertexLayout,
    ReadTexture,
    ReadFrameBuffer
};

struct CreateFrameBufferCommand : Foundation::CommandBuffer::Command {
    FrameBufferHandle handle;
    FrameBufferSpecification spec;

    CreateFrameBufferCommand(FrameBufferHandle handle, FrameBufferSpecification spec)
        : Command(RendererCommandType::CreateFrameBuffer)
        , handle(handle)
        , spec(spec) {}
};

struct DeleteFrameBufferCommand : Foundation::CommandBuffer::Command {
    FrameBufferHandle handle;

    DeleteFrameBufferCommand(FrameBufferHandle handle)
        : Command(RendererCommandType::DestroyFrameBuffer)
        , handle(handle) {}
};

struct CreateProgramCommand : Foundation::CommandBuffer::Command {
    ProgramCreate create;
    ProgramHandle handle;

    CreateProgramCommand(ProgramHandle handle, ProgramCreate create)
        : Command(RendererCommandType::CreateProgram)
        , handle(handle)
        , create(create) {}
};

struct DeleteProgramCommand : Foundation::CommandBuffer::Command {
    ProgramHandle handle;

    DeleteProgramCommand(ProgramHandle handle)
        : Command(RendererCommandType::DestroyProgram)
        , handle(handle) {}
};

struct CreateTextureCommand : Foundation::CommandBuffer::Command {
    TextureHandle handle;
    TextureCreate create;

    CreateTextureCommand(TextureHandle handle, TextureCreate create)
        : Command(RendererCommandType::CreateTexture)
        , handle(handle)
        , create(create) {}
};

struct ResizeTextureCommand : Foundation::CommandBuffer::Command {
    TextureHandle handle;
    uint32_t width;
    uint32_t height;

    ResizeTextureCommand(TextureHandle handle, uint32_t width, uint32_t height)
        : Command(RendererCommandType::ResizeTexture)
        , handle(handle)
        , width(width)
        , height(height) {}
};

struct DeleteTextureCommand : Foundation::CommandBuffer::Command {
    TextureHandle handle;

    DeleteTextureCommand(TextureHandle handle)
        : Command(RendererCommandType::DestroyTexture)
        , handle(handle) {}
};

struct CreateIndexBufferCommand : Foundation::CommandBuffer::Command {
    IndexBufferHandle handle;
    Foundation::Memory indices;
    BufferElementType elementType;
    size_t count;

    CreateIndexBufferCommand(
        IndexBufferHandle handle,
        Foundation::Memory indices,
        BufferElementType elementType,
        size_t count
    )
        : Command(RendererCommandType::CreateIndexBuffer)
        , handle(handle)
        , indices(indices)
        , elementType(elementType)
        , count(count) {}
};

struct CreateDynamicIndexBufferCommand : Foundation::CommandBuffer::Command {
    IndexBufferHandle handle;
    Foundation::Memory indices;
    BufferElementType elementType;
    size_t count;

    CreateDynamicIndexBufferCommand(
        IndexBufferHandle handle,
        Foundation::Memory indices,
        BufferElementType elementType,
        size_t count
    )
        : Command(RendererCommandType::CreateDynamicIndexBuffer)
        , handle(handle)
        , indices(indices)
        , elementType(elementType)
        , count(count) {}
};

struct UpdateDynamicIndexBufferCommand : Foundation::CommandBuffer::Command {
    IndexBufferHandle handle;
    Foundation::Memory indices;
    size_t count;

    UpdateDynamicIndexBufferCommand(
        IndexBufferHandle handle, Foundation::Memory indices, size_t count
    )
        : Command(RendererCommandType::UpdateDynamicIndexBuffer)
        , handle(handle)
        , indices(indices)
        , count(count) {}
};

struct DeleteIndexBufferCommand : Foundation::CommandBuffer::Command {
    IndexBufferHandle handle;

    DeleteIndexBufferCommand(IndexBufferHandle handle)
        : Command(RendererCommandType::DestroyIndexBuffer)
        , handle(handle) {}
};

struct CreateVertexBufferCommand : Foundation::CommandBuffer::Command {
    VertexBufferHandle handle;
    VertexLayoutHandle layoutHandle;
    Foundation::Memory data;
    uint32_t size;

    CreateVertexBufferCommand(
        VertexBufferHandle handle,
        Foundation::Memory data,
        uint32_t size,
        VertexLayoutHandle layoutHandle
    )
        : Command(RendererCommandType::CreateVertexBuffer)
        , handle(handle)
        , layoutHandle(layoutHandle)
        , data(data)
        , size(size) {}
};

struct CreateDynamicVertexBufferCommand : Foundation::CommandBuffer::Command {
    VertexBufferHandle handle;
    VertexLayoutHandle layoutHandle;
    Foundation::Memory data;
    uint32_t size;

    CreateDynamicVertexBufferCommand(
        VertexBufferHandle handle,
        Foundation::Memory data,
        uint32_t size,
        VertexLayoutHandle layoutHandle
    )
        : Command(RendererCommandType::CreateDynamicVertexBuffer)
        , handle(handle)
        , layoutHandle(layoutHandle)
        , data(data)
        , size(size) {}
};

struct UpdateDynamicVertexBufferCommand : Foundation::CommandBuffer::Command {
    VertexBufferHandle handle;
    Foundation::Memory data;
    uint32_t size;

    UpdateDynamicVertexBufferCommand(
        VertexBufferHandle handle, Foundation::Memory data, uint32_t size
    )
        : Command(RendererCommandType::UpdateDynamicVertexBuffer)
        , handle(handle)
        , data(data)
        , size(size) {}
};

struct DeleteVertexBufferCommand : Foundation::CommandBuffer::Command {
    VertexBufferHandle handle;

    DeleteVertexBufferCommand(VertexBufferHandle handle)
        : Command(RendererCommandType::DestroyVertexBuffer)
        , handle(handle) {}
};

struct CreateVertexLayoutCommand : Foundation::CommandBuffer::Command {
    VertexBufferLayoutData data;
    VertexLayoutHandle handle;

    CreateVertexLayoutCommand(VertexLayoutHandle handle, VertexBufferLayoutData data)
        : Command(RendererCommandType::CreateVertexLayout)
        , handle(handle)
        , data(data) {}
};

struct DeleteVertexLayoutCommand : Foundation::CommandBuffer::Command {
    VertexLayoutHandle handle;

    DeleteVertexLayoutCommand(VertexLayoutHandle handle)
        : Command(RendererCommandType::DestroyVertexLayout)
        , handle(handle) {}
};

struct ReadTextureCommand : Foundation::CommandBuffer::Command {
    TextureHandle handle;
    void *data;

    ReadTextureCommand(TextureHandle handle, void *data)
        : Command(RendererCommandType::ReadTexture)
        , handle(handle)
        , data(data) {}
};

struct ReadFrameBufferCommand : Foundation::CommandBuffer::Command {
    FrameBufferHandle handle;
    int attachIndex;
    int x, y, width, height;
    void *data;

    ReadFrameBufferCommand(
        FrameBufferHandle handle, int attachIndex, int x, int y, int width, int height, void *data
    )
        : Command(RendererCommandType::ReadFrameBuffer)
        , handle(handle)
        , attachIndex(attachIndex)
        , x(x)
        , y(y)
        , width(width)
        , height(height)
        , data(data) {}
};

} // namespace Bird