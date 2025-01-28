//
// Created by Admin on 11.02.2022.
//

#pragma once

#include <Foundation/Assert.hpp>
#include <Foundation/Logger.hpp>

namespace Bird {

#define MAX_VERTEX_LAYOUT_ELEMENTS 20

enum class BufferElementType { Float, UnsignedInt, Int, UnsignedShort, UnsignedByte };

struct VertexBufferElement {
    BufferElementType type;
    uint32_t count;
    bool normalized;

    static size_t getSizeOfType(BufferElementType type) {
        switch (type) {
            case BufferElementType::Float:
                return sizeof(float);
            case BufferElementType::UnsignedInt:
                return sizeof(unsigned int);
            case BufferElementType::Int:
                return sizeof(int);
            case BufferElementType::UnsignedShort:
                return sizeof(unsigned short);
            case BufferElementType::UnsignedByte:
                return sizeof(unsigned char);
        }
        NEST_ASSERT(false, "Vertex buffer element type is undefined");
        return 0;
    }
};

class VertexBufferLayoutData {
public:
    VertexBufferLayoutData()
        : m_stride(0)
        , m_elementsCount(0)
        , m_elements() {}

    virtual ~VertexBufferLayoutData() = default;

    void pushFloat(uint32_t count) {
        VertexBufferElement element = {BufferElementType::Float, count, false};
        m_elements[m_elementsCount++] = element;
        m_stride += count * VertexBufferElement::getSizeOfType(BufferElementType::Float);
    }

    void pushUInt(uint32_t count) {
        VertexBufferElement element = {BufferElementType::UnsignedInt, count, false};
        m_elements[m_elementsCount++] = element;
        m_stride += count * VertexBufferElement::getSizeOfType(BufferElementType::UnsignedInt);
    }

    void pushInt(uint32_t count) {
        VertexBufferElement element = {BufferElementType::Int, count, false};
        m_elements[m_elementsCount++] = element;
        m_stride += count * VertexBufferElement::getSizeOfType(BufferElementType::Int);
    }

    void pushChar(uint32_t count, bool normalized) {
        VertexBufferElement element = {BufferElementType::UnsignedByte, count, normalized};
        m_elements[m_elementsCount++] = element;
        m_stride += count * VertexBufferElement::getSizeOfType(BufferElementType::UnsignedByte);
    }

    void pushVec3() {
        VertexBufferElement element = {BufferElementType::Float, 3, false};
        m_elements[m_elementsCount++] = element;
        m_stride += VertexBufferElement::getSizeOfType(BufferElementType::Float) * 3;
    }

    void pushVec4() {
        VertexBufferElement element = {BufferElementType::Float, 4, false};
        m_elements[m_elementsCount++] = element;
        m_stride += VertexBufferElement::getSizeOfType(BufferElementType::Float) * 4;
    }

    void push8BitRGBAColor() {
        VertexBufferElement element = {BufferElementType::UnsignedByte, 4, true};
        m_elements[m_elementsCount++] = element;
        m_stride += VertexBufferElement::getSizeOfType(BufferElementType::UnsignedByte) * 4;
    }

    void pushVec2() {
        VertexBufferElement element = {BufferElementType::Float, 2, false};
        m_elements[m_elementsCount++] = element;
        m_stride += VertexBufferElement::getSizeOfType(BufferElementType::Float) * 2;
    }

    VertexBufferElement m_elements[MAX_VERTEX_LAYOUT_ELEMENTS];
    uint32_t m_elementsCount;
    uint32_t m_stride;
};

} // namespace Bird
