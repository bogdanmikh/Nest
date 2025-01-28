#pragma once

#include "Bird/Base.hpp"

namespace Bird {

struct View {
    View()
        : m_clearColor(0x000000ff)
        , m_viewport(Rect::zero())
        , m_frameBuffer(BIRD_INVALID_HANDLE)
        , m_clearAttachments() {}

    uint32_t m_clearColor;
    Rect m_viewport;
    FrameBufferHandle m_frameBuffer;
    std::vector<Clear> m_clearAttachments;
};

} // namespace Bird
