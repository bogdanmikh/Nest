//
// Created by Admin on 08.03.2022.
//

#pragma once

#include "Base.hpp"
#include "Encoder/View.hpp"
#include "Encoder/Frame.hpp"
#include "Encoder/Uniform.hpp"
#include "VertexBufferLayoutData.hpp"

namespace NestRen {

enum class RendererType {
    Noop, //!< No rendering.
    // Direct3D12,   //!< Direct3D 12.0
    // Metal,        //!< Metal
    OpenGLES, //!< OpenGL ES 2.0+
    OpenGL,   //!< OpenGL 2.1+
    // Vulkan,       //!< Vulkan
};

class RendererI {
public:
    virtual ~RendererI() = default;
    virtual RendererType getRendererType() const = 0;
    virtual void flip() = 0;
    virtual void
    createFrameBuffer(FrameBufferHandle handle, FrameBufferSpecification specification) = 0;
    virtual void deleteFrameBuffer(FrameBufferHandle handle) = 0;
    virtual void createProgram(ProgramHandle handle, ProgramCreate) = 0;
    virtual void deleteShader(ProgramHandle handle) = 0;
    virtual void createTexture(TextureHandle handle, TextureCreate) = 0;
    virtual void resizeTexture(TextureHandle handle, uint32_t width, uint32_t height) = 0;
    virtual void deleteTexture(TextureHandle handle) = 0;
    virtual void createIndexBuffer(
        IndexBufferHandle handle,
        Memory indices,
        BufferElementType elementType,
        size_t count
    ) = 0;
    virtual void createDynamicIndexBuffer(
        IndexBufferHandle handle,
        Memory indices,
        BufferElementType elementType,
        size_t count
    ) = 0;
    virtual void updateDynamicIndexBuffer(
        IndexBufferHandle handle, Memory indices, size_t count
    ) = 0;
    virtual void deleteIndexBuffer(IndexBufferHandle handle) = 0;
    virtual void createVertexBuffer(
        VertexBufferHandle handle,
        Memory data,
        uint32_t size,
        VertexLayoutHandle layoutHandle
    ) = 0;
    virtual void createDynamicVertexBuffer(
        VertexBufferHandle handle,
        Memory data,
        uint32_t size,
        VertexLayoutHandle layoutHandle = INVALID_HANDLE
    ) = 0;
    virtual void updateDynamicVertexBuffer(
        VertexBufferHandle handle, Memory data, uint32_t size
    ) = 0;
    virtual void deleteVertexBuffer(VertexBufferHandle handle) = 0;
    virtual void createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) = 0;
    virtual void deleteVertexLayout(VertexLayoutHandle handle) = 0;
    virtual void setUniform(const Uniform &uniform) = 0;
    virtual void setTexture(TextureHandle handle, uint32_t slot) = 0;
    virtual void submit(Frame *frame, View *views) = 0;
};

} // namespace Miren
