#pragma once

#include "NestRen/Base.hpp"

#include <vector>

namespace NestRen {

class VulkanFrameBuffer {
public:
    VulkanFrameBuffer();
    void create(FrameBufferSpecification specification);
    void terminate();
    void bind();
    void unbind();

private:
    void checkStatus();

    FrameBufferSpecification spec;
    uint32_t m_id;
};

} // namespace NestRen
