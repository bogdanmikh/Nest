//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Bird/VertexBufferLayoutData.hpp"

namespace Bird {

class OpenGLIndexBuffer {
public:
    OpenGLIndexBuffer();
    void create(void *indices, BufferElementType elementType, size_t count, bool isDynamic);
    void terminate();
    void update(void *indices, size_t count);
    void bind() const;
    void unbind() const;
    uint32_t getCount() const;
    inline uint32_t getElementType() {
        return m_elementType;
    };

private:
    uint32_t m_id;
    uint32_t m_count;
    uint32_t m_elementType;
    size_t m_elementSize;
    bool m_isDynamic;
};

} // namespace Bird
