#pragma once

#include "Bird/Base.hpp"

#include <vector>

namespace Bird {

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

} // namespace Bird
