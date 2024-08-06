#pragma once

#include "Bird/Base.hpp"

namespace Bird {

struct View {
    View()
        : m_clearColor(0x00ff00ff)
        , m_viewport(Rect::zero())
        , m_frameBuffer(BIRD_INVALID_HANDLE) {}

    uint32_t m_clearColor;
    Rect m_viewport;
    FrameBufferHandle m_frameBuffer;
};

} // namespace Bird
