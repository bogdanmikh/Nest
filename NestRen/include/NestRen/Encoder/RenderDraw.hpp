//
// Created by Admin on 13.03.2022.
//

#pragma once

#include "Miren/Base.hpp"
#include "Miren/MirenStates.hpp"
#include "Uniform.hpp"

namespace NestRen {

struct TextureBinding {
    TextureHandle m_handle;
    uint32_t m_slot;

    TextureBinding(TextureHandle mHandle, uint32_t mSlot)
        : m_handle(mHandle)
        , m_slot(mSlot) {}

    TextureBinding()
        : m_handle(0)
        , m_slot(0) {}
};

// Вызов отрисовки кадра
struct RenderDraw {
    RenderDraw()
        : m_isSubmitted(false)
        , m_state(MIREN_STATE_CULL_FACE | MIREN_STATE_DEPTH_TEST)
        , m_numIndices(0)
        , m_uniformsCount(0)
        , m_textureBindingsCount(0)
        , m_indicesOffset(0)
        , m_verticesOffset(0)
        , m_viewId(0)
        , m_shader(MIREN_INVALID_HANDLE)
        , m_indexBuffer(MIREN_INVALID_HANDLE)
        , m_vertexBuffer(MIREN_INVALID_HANDLE)
        , m_vertexLayout(MIREN_INVALID_HANDLE)
        , m_scissorRect(Rect::zero()) {}

    void reset() {
        m_isSubmitted = false;
        m_state = MIREN_STATE_CULL_FACE | MIREN_STATE_DEPTH_TEST;
        m_numIndices = 0;
        m_uniformsCount = 0;
        m_textureBindingsCount = 0;
        m_indicesOffset = 0;
        m_verticesOffset = 0;
        m_viewId = 0;
        m_shader = MIREN_INVALID_HANDLE;
        m_indexBuffer = MIREN_INVALID_HANDLE;
        m_vertexBuffer = MIREN_INVALID_HANDLE;
        m_vertexLayout = MIREN_INVALID_HANDLE;
        m_scissorRect = Rect::zero();
    }

    void addUniform(ProgramHandle handle, const char *name, void *value, UniformDataType type) {
        m_uniformBuffer[m_uniformsCount++] = Uniform(handle, name, value, type);
    }

    void setTexture(TextureHandle textureHandle, uint32_t slot) {
        m_textureBindings[m_textureBindingsCount++] = TextureBinding(textureHandle, slot);
    }

    bool m_isSubmitted;
    uint32_t m_numIndices;
    intptr_t m_indicesOffset;
    intptr_t m_verticesOffset;
    ProgramHandle m_shader;
    IndexBufferHandle m_indexBuffer;
    VertexBufferHandle m_vertexBuffer;
    VertexLayoutHandle m_vertexLayout;
    uint32_t m_uniformsCount;
    Uniform m_uniformBuffer[MAX_UNIFORMS];
    uint32_t m_textureBindingsCount;
    TextureBinding m_textureBindings[MAX_TEXTURE_BINDINGS];
    Rect m_scissorRect;
    ViewId m_viewId;
    uint32_t m_state;
};

} // namespace Miren