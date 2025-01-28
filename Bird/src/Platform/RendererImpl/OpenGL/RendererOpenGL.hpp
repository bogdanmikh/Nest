//
// Created by Admin on 14.03.2022.
//

#pragma once

#include "Bird/RendererI.hpp"
#include "Bird/GraphicsContext.hpp"
#include "Platform/RendererImpl/OpenGL/Extensions/OpenGLExtensions.hpp"
#include "OpenGLFrameBuffer.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLIndexBuffer.hpp"
#include "OpenGLVertexBuffer.hpp"

namespace Bird {

class RendererOpenGL : public RendererI {
public:
    RendererOpenGL();
    ~RendererOpenGL() override;
    RendererType getRendererType() const override;
    void flip() override;
    void
    createFrameBuffer(FrameBufferHandle handle, FrameBufferSpecification specification) override;
    void readFrameBuffer(
        Bird::FrameBufferHandle handle,
        int attachIndex,
        int x,
        int y,
        int width,
        int height,
        void *data
    ) override;
    void deleteFrameBuffer(FrameBufferHandle handle) override;
    void createProgram(ProgramHandle handle, ProgramCreate create) override;
    void deleteShader(ProgramHandle handle) override;
    void createTexture(TextureHandle handle, TextureCreate create) override;
    void resizeTexture(TextureHandle handle, uint32_t width, uint32_t height) override;
    void deleteTexture(TextureHandle handle) override;
    void createIndexBuffer(
        IndexBufferHandle handle,
        Foundation::Memory indices,
        BufferElementType elementType,
        size_t count
    ) override;
    void createDynamicIndexBuffer(
        IndexBufferHandle handle,
        Foundation::Memory indices,
        BufferElementType elementType,
        size_t count
    ) override;
    void updateDynamicIndexBuffer(
        IndexBufferHandle handle, Foundation::Memory indices, size_t count
    ) override;
    void deleteIndexBuffer(IndexBufferHandle handle) override;
    void createVertexBuffer(
        VertexBufferHandle handle,
        Foundation::Memory data,
        uint32_t size,
        VertexLayoutHandle layoutHandle
    ) override;
    void createDynamicVertexBuffer(
        VertexBufferHandle handle,
        Foundation::Memory data,
        uint32_t size,
        VertexLayoutHandle layoutHandle
    ) override;
    void updateDynamicVertexBuffer(
        VertexBufferHandle handle, Foundation::Memory data, uint32_t size
    ) override;
    void deleteVertexBuffer(VertexBufferHandle handle) override;
    void createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) override;
    void deleteVertexLayout(VertexLayoutHandle handle) override;
    void readTexture(Bird::TextureHandle handle, void *data) override;
    void setUniform(const Uniform &uniform) override;
    void setTexture(TextureHandle handle, uint32_t slot) override;
    void submit(Frame *frame, View *views) override;

    static RendererOpenGL *s_instance;
    OpenGLTexture &getTexture(TextureHandle handle) {
        return textures[handle.id];
    }

private:
    void viewChanged(View &view);
    void submit(RenderDraw *draw);

    uint32_t m_uselessVao;
    GraphicsContext *context;
    OpenGLFrameBuffer frameBuffers[1000];
    OpenGLShader shaders[1000];
    OpenGLIndexBuffer indexBuffers[1000];
    VertexBufferLayoutData vertexLayouts[1000];
    OpenGLVertexBuffer vertexBuffers[1000];
    OpenGLTexture textures[1000];
};

} // namespace Bird
