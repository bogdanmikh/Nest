#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <vector>

struct Vertex {
    // position
    glm::vec3 Position;
    // texCoords
    glm::vec2 TexCoords;
    // light
    float light;

    Vertex()
        : Position(0, 0, 0)
        , TexCoords(0, 0)
        , light(0) {};

    Vertex(glm::vec3 aPosition, glm::vec2 aTexCoords, float light = 1.f)
        : Position(aPosition)
        , TexCoords(aTexCoords)
        , light(light) {};

    Vertex(float x, float y, float z, float aTexCoordX, float aTexCoordY, float light = 1.f)
        : Position(x, y, z)
        , TexCoords(aTexCoordX, aTexCoordY)
        , light(light) {};
};

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT:
                return 4;
            case GL_UNSIGNED_INT:
                return 4;
            case GL_UNSIGNED_BYTE:
                return 1;
        }
        assert(false);
        return 0;
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    VertexBufferLayout()
        : m_Stride(0) {}
    ~VertexBufferLayout() {};

    void pushFloat(unsigned int count = 1) {
        VertexBufferElement element = {GL_FLOAT, count, GL_FALSE};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
    }

    void pushUInt(unsigned int count = 1) {
        VertexBufferElement element = {GL_UNSIGNED_INT, count, GL_FALSE};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
    }

    void pushChar(unsigned int count = 1) {
        VertexBufferElement element = {GL_UNSIGNED_BYTE, count, GL_TRUE};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
    }

    void pushVec3F(unsigned int count = 1) {
        VertexBufferElement element = {GL_FLOAT, count * 3, GL_FALSE};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT) * 3;
    }

    void pushVec2F(unsigned int count = 1) {
        VertexBufferElement element = {GL_FLOAT, count * 2, GL_FALSE};
        m_Elements.push_back(element);
        m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT) * 2;
    }

    void pushVertex(unsigned int count = 1) {
        pushVec3F(1);
        pushVec2F(1);
        pushFloat(1);
    }

    inline std::vector<VertexBufferElement> getElements() const {
        return m_Elements;
    }

    inline unsigned int getStride() const {
        return m_Stride;
    }
};
