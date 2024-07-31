//
// Created by Admin on 14.03.2022.
//

#pragma once

#include "NestRen/Base.hpp"
#include "Nest/Allocator/Allocator.hpp"
#include "NestRen/VertexBufferLayoutData.hpp"
#include "NestRen/Encoder/RenderDraw.hpp"

namespace NestRen {

template<typename T, uint32_t Max>
class FreeHandleQueue {
public:
    FreeHandleQueue()
        : m_num(0) {}

    void queue(T handle) {
        m_queue[m_num] = handle;
        ++m_num;
    }

    void reset() {
        m_num = 0;
    }

    T get(uint16_t id) const {
        return m_queue[id];
    }

    uint16_t getNumQueued() const {
        return m_num;
    }

private:
    T m_queue[Max];
    uint16_t m_num;
};

template<typename T>
class HandleAllocator {
public:
    explicit HandleAllocator(uint16_t maxHandles)
        : m_numHandles(0)
        , m_maxHandles(maxHandles) {
        m_dense = new T[m_maxHandles];
        m_sparse = new uint16_t[m_maxHandles];
        reset();
    }

    ~HandleAllocator() {
        delete[] m_dense;
        delete[] m_sparse;
    }

    T getHandleAt(uint16_t at) const {
        return m_dense[at];
    }

    uint16_t getNumHandles() const {
        return m_numHandles;
    }

    uint16_t getMaxHandles() const {
        return m_maxHandles;
    }

    T alloc() {
        if (m_numHandles < m_maxHandles) {
            uint16_t index = m_numHandles;
            ++m_numHandles;

            T handle = m_dense[index];
            m_sparse[handle.id] = index;
            return handle;
        }

        return NESTREN_INVALID_HANDLE;
    }

    bool isValid(T handle) const {
        uint16_t index = m_sparse[handle];
        return index < m_numHandles && m_dense[index] == handle;
    }

    void free(T handle) {
        uint16_t index = m_sparse[handle.id];
        --m_numHandles;
        T temp = m_dense[m_numHandles];
        m_dense[m_numHandles] = handle;
        m_sparse[temp.id] = index;
        m_dense[index] = temp;
    }

    void reset() {
        m_numHandles = 0;
        for (uint16_t i = 0; i < m_maxHandles; ++i) {
            m_dense[i] = i;
        }
    }

private:
    T *m_dense;
    uint16_t *m_sparse;
    uint16_t m_numHandles;
    uint16_t m_maxHandles;
};
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
    void setIsIndexed(bool value);
    void setNumberOfElements(uint32_t count);
    void setScissorRect(Rect rect);
    void setUniform(ProgramHandle handle, const char *name, void *value, UniformDataType type);
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
};

} // namespace Miren
