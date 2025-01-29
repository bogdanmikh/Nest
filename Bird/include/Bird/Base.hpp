#pragma once

#include "Config.hpp"
#include "VertexBufferLayoutData.hpp"

#include <Foundation/Foundation.hpp>
#include <cstdlib>

#if BIRD_LOG_ENABLED == 1
#    define BIRD_LOG(...) LOG_INFO(__VA_ARGS__)
#else
#    define BIRD_LOG(...)
#endif

#if BIRD_CMDBUF_LOG_ENABLED == 1
#    define CMDBUF_LOG(...) LOG_INFO(__VA_ARGS__)
#else
#    define CMDBUF_LOG(...)
#endif

#define BIRD_INVALID_HANDLE UINT16_MAX

namespace Bird {

#define BIRD_HANDLE(name)                                                                          \
    struct name {                                                                                  \
        name(uint16_t id)                                                                          \
            : id(id) {}                                                                            \
        name()                                                                                     \
            : id(BIRD_INVALID_HANDLE) {}                                                           \
        uint16_t id;                                                                               \
        bool isValid() {                                                                           \
            return id != BIRD_INVALID_HANDLE;                                                      \
        }                                                                                          \
    };

using ViewId = uint16_t;
BIRD_HANDLE(ProgramHandle)
BIRD_HANDLE(TextureHandle)
BIRD_HANDLE(IndexBufferHandle)
BIRD_HANDLE(FrameBufferHandle)
BIRD_HANDLE(VertexBufferHandle)
BIRD_HANDLE(VertexLayoutHandle)

enum TextureFormat {
    // Color
    RGB8,
    RGBA8,
    RED_INTEGER,
    // Depth/stencil
    DEPTH24STENCIL8
};

enum TextureWrapMode { CLAMP, REPEAT };

enum TextureFiltering {
    NEAREST,
    LINEAR,
    NEAREST_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_NEAREST,
    LINEAR_MIPMAP_LINEAR
};

struct ProgramCreate {
    Foundation::Memory m_vertex;
    Foundation::Memory m_fragment;
};

struct TextureCreate {
    TextureFormat m_format;
    TextureFiltering m_minFiltering;
    TextureFiltering m_magFiltering;
    TextureWrapMode m_wrap;
    uint16_t m_width;
    uint16_t m_height;
    uint8_t m_numMips;
    bool m_isCubeMap;
    Foundation::Memory m_data;

    TextureCreate()
        : m_format(TextureFormat::RGBA8)
        , m_minFiltering(TextureFiltering::NEAREST)
        , m_magFiltering(TextureFiltering::NEAREST)
        , m_wrap(TextureWrapMode::REPEAT)
        , m_width(1)
        , m_height(1)
        , m_numMips(0)
        , m_isCubeMap(false)
        , m_data(nullptr) {}

    size_t bytesPerTexel() const {
        switch (m_format) {
            case RGB8:
                return 3;
            case RGBA8:
                return 4;
            case RED_INTEGER:
                return 4;
            case DEPTH24STENCIL8:
                return 4;
        }
    }
};

enum UniformType { Sampler, Vec4, Mat3, Mat4, Count };

struct FrameBufferAttachment {
    FrameBufferAttachment()
        : handle(BIRD_INVALID_HANDLE) {}

    FrameBufferAttachment(TextureHandle handle)
        : handle(handle) {}

    TextureHandle handle;
};

struct FrameBufferSpecification {
    FrameBufferSpecification()
        : attachments()
        , num(0) {}

    FrameBufferSpecification(FrameBufferAttachment *_attachments, uint32_t num)
        : attachments()
        , num(num) {
        NEST_ASSERT(num <= MAX_FRAMEBUFFER_ATTACHMENTS, "MAX ATTACHMENTS NUMBER EXCEEDED");
        for (int i = 0; i < num; i++) {
            attachments[i] = _attachments[i];
        }
    }

    FrameBufferAttachment attachments[MAX_FRAMEBUFFER_ATTACHMENTS];
    uint32_t num;
};

struct TransientIndexBuffer {
    TransientIndexBuffer()
        : data(nullptr)
        , size(0)
        , startIndex(0)
        , handle(BIRD_INVALID_HANDLE)
        , elementType(BufferElementType::UnsignedInt) {}
    uint8_t *data;
    uint32_t size;
    uint32_t startIndex;
    IndexBufferHandle handle;
    BufferElementType elementType;
};

struct TransientVertexBuffer {
    TransientVertexBuffer()
        : data(nullptr)
        , size(0)
        , startVertex(0)
        , handle(BIRD_INVALID_HANDLE) {}
    uint8_t *data;
    uint32_t size;
    uint32_t startVertex;
    VertexBufferHandle handle;
};

struct Size {
    int width;
    int height;

    Size()
        : width(0)
        , height(0) {}

    Size(int width, int height)
        : width(width)
        , height(height) {}

    inline bool isZero() {
        return width == 0 && height == 0;
    }
};

struct Point {
    int x;
    int y;

    Point()
        : x(0)
        , y(0) {}

    Point(int x, int y)
        : x(x)
        , y(y) {}

    inline bool isZero() const {
        return x == 0 && y == 0;
    }
};

struct Rect {
    Point origin;
    Size size;

    Rect()
        : origin()
        , size() {}

    Rect(int x, int y, int width, int height)
        : origin(x, y)
        , size(width, height) {}

    inline static Rect zero() {
        return {};
    }

    inline bool isZero() {
        return origin.isZero() && size.isZero();
    }
};

struct Clear {
    int attachmentIndex;
    int value;

    Clear(int attachmentIndex, int value)
        : attachmentIndex(attachmentIndex)
        , value(value) {}
};

} // namespace Bird
