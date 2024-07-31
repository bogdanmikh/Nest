#pragma once

#include "NestRen/Base.hpp"
#include "Nest/Base/Base.hpp"

namespace NestRen {

struct View {
    View()
        : m_clearColor(0x000000ff)
        , m_viewport({0, 0})
        , m_frameBuffer(INVALID_HANDLE) {}

    uint32_t m_clearColor;
    Vec2 m_viewport;
    FrameBufferHandle m_frameBuffer;
};

} // namespace Miren
