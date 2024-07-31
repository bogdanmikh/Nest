//
// Created by Admin on 05.03.2022.
//

#pragma once

#include "NestRen/VertexBufferLayoutData.hpp"
#include "NestRen/Base.hpp"

namespace NestRen {

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
    DestroyVertexLayout
};

struct CreateFrameBufferCommand {
    FrameBufferHandle handle;
    FrameBufferSpecification spec;

    CreateFrameBufferCommand(FrameBufferHandle handle, FrameBufferSpecification spec)
        : handle(handle)
        , spec(spec) {}
};

struct DeleteFrameBufferCommand {
    FrameBufferHandle handle;

    DeleteFrameBufferCommand(FrameBufferHandle handle)
        : handle(handle) {}
};

struct CreateProgramCommand {
    ProgramCreate create;
    ProgramHandle handle;

    CreateProgramCommand(ProgramHandle handle, ProgramCreate create)
        : handle(handle)
        , create(create) {}
};

struct DeleteProgramCommand {
    ProgramHandle handle;

    DeleteProgramCommand(ProgramHandle handle)
        : handle(handle) {}
};

struct CreateTextureCommand {
    TextureHandle handle;
    TextureCreate create;

    CreateTextureCommand(TextureHandle handle, TextureCreate create)
        : handle(handle)
        , create(create) {}
};

struct ResizeTextureCommand {
    TextureHandle handle;
    uint32_t width;
    uint32_t height;

    ResizeTextureCommand(TextureHandle handle, uint32_t width, uint32_t height)
        : handle(handle)
        , width(width)
        , height(height) {}
};

struct DeleteTextureCommand {
    TextureHandle handle;

    DeleteTextureCommand(TextureHandle handle)
        : handle(handle) {}
};

struct CreateIndexBufferCommand {
    IndexBufferHandle handle;
    Memory indices;
    BufferElementType elementType;
    size_t count;

    CreateIndexBufferCommand(
        IndexBufferHandle handle, Memory indices, BufferElementType elementType, size_t count
    )
        : handle(handle)
        , indices(indices)
        , elementType(elementType)
        , count(count) {}
};

struct CreateDynamicIndexBufferCommand {
    IndexBufferHandle handle;
    Memory indices;
    BufferElementType elementType;
    size_t count;

    CreateDynamicIndexBufferCommand(
        IndexBufferHandle handle, Memory indices, BufferElementType elementType, size_t count
    )
        : handle(handle)
        , indices(indices)
        , elementType(elementType)
        , count(count) {}
};

struct UpdateDynamicIndexBufferCommand {
    IndexBufferHandle handle;
    Memory indices;
    size_t count;

    UpdateDynamicIndexBufferCommand(IndexBufferHandle handle, Memory indices, size_t count)
        : handle(handle)
        , indices(indices)
        , count(count) {}
};

struct DeleteIndexBufferCommand {
    IndexBufferHandle handle;

    DeleteIndexBufferCommand(IndexBufferHandle handle)
        : handle(handle) {}
};

struct CreateVertexBufferCommand {
    VertexBufferHandle handle;
    VertexLayoutHandle layoutHandle;
    Memory data;
    uint32_t size;

    CreateVertexBufferCommand(
        VertexBufferHandle handle, Memory data, uint32_t size, VertexLayoutHandle layoutHandle
    )
        : handle(handle)
        , layoutHandle(layoutHandle)
        , data(data)
        , size(size) {}
};

struct CreateDynamicVertexBufferCommand {
    VertexBufferHandle handle;
    VertexLayoutHandle layoutHandle;
    Memory data;
    uint32_t size;

    CreateDynamicVertexBufferCommand(
        VertexBufferHandle handle, Memory data, uint32_t size, VertexLayoutHandle layoutHandle
    )
        : handle(handle)
        , layoutHandle(layoutHandle)
        , data(data)
        , size(size) {}
};

struct UpdateDynamicVertexBufferCommand {
    VertexBufferHandle handle;
    Memory data;
    uint32_t size;

    UpdateDynamicVertexBufferCommand(VertexBufferHandle handle, Memory data, uint32_t size)
        : handle(handle)
        , data(data)
        , size(size) {}
};

struct DeleteVertexBufferCommand {
    VertexBufferHandle handle;

    DeleteVertexBufferCommand(VertexBufferHandle handle)
        : handle(handle) {}
};

struct CreateVertexLayoutCommand {
    VertexBufferLayoutData data;
    VertexLayoutHandle handle;

    CreateVertexLayoutCommand(VertexLayoutHandle handle, VertexBufferLayoutData data)
        : handle(handle)
        , data(data) {}
};

struct DeleteVertexLayoutCommand {
    VertexLayoutHandle handle;

    DeleteVertexLayoutCommand(VertexLayoutHandle handle)
        : handle(handle) {}
};

} // namespace NestRen