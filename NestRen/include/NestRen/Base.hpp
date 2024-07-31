#pragma once

#include "Nest/Allocator/Memory.hpp"
#include "Nest/Logger/Assert.hpp"

#define INVALID_HANDLE UINT16_MAX
#define MAX_FRAMEBUFFER_ATTACHMENTS 5

namespace NestRen {

#define NESTREN_HANDLE(name)                                                                         \
    struct name {                                                                                  \
        name(uint16_t id)                                                                          \
            : id(id) {}                                                                            \
        name()                                                                                     \
            : id(INVALID_HANDLE) {}                                                          \
        uint16_t id;                                                                               \
        bool isValid() {                                                                           \
            return id != INVALID_HANDLE;                                                     \
        }                                                                                          \
    };

using ViewId = uint16_t;
NESTREN_HANDLE(IndexBufferHandle)
NESTREN_HANDLE(VertexBufferHandle)
NESTREN_HANDLE(ProgramHandle)
NESTREN_HANDLE(TextureHandle)
NESTREN_HANDLE(FrameBufferHandle)
NESTREN_HANDLE(VertexLayoutHandle)

using size = uint32_t;

struct ProgramCreate {
    Memory m_vertex;
    Memory m_fragment;
};

enum TextureFormat {
    None = 0,
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

enum class UniformDataType { Int, Mat4, IntArray };

struct FrameBufferAttachment {
    FrameBufferAttachment()
        : handle(INVALID_HANDLE) {}

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

struct TextureCreate {
    TextureFormat m_format;
    TextureFiltering m_minFiltering;
    TextureFiltering m_magFiltering;
    TextureWrapMode m_wrap;
    uint16_t m_width;
    uint16_t m_height;
    uint8_t m_numMips;
    bool m_isCubeMap;
    Memory m_data;

    TextureCreate()
        : m_format(TextureFormat::None)
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
            case None:
                return 0;
            case RGB8:
                return 3;
            case RGBA8:
                return 4;
            case RED_INTEGER:
                return 1;
            case DEPTH24STENCIL8:
                return 4;
        }
    }
};

}