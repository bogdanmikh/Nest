#pragma once

#include "NestRen/Base.hpp"

namespace NestRen {

struct View {
    View()
        : m_clearColor(0x000000ff)
        , m_viewport(Rect::zero())
        , m_frameBuffer(NESTREN_INVALID_HANDLE) {}

    uint32_t m_clearColor;
    Rect m_viewport;
    FrameBufferHandle m_frameBuffer;
};

} // namespace Miren
