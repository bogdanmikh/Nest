#pragma once

#include "Bird/Base.hpp"

#include <vector>

namespace Bird {

class OpenGLFrameBuffer {
public:
    OpenGLFrameBuffer();
    void create(FrameBufferSpecification specification);
    void terminate();
    void bind();
    void unbind();
    void readPixels(int index, int x, int y, int width, int height, void *data);
    void clearIntAttachment(int index, int value);

private:
    void checkStatus();

    FrameBufferSpecification spec;
    uint32_t m_id;
};

} // namespace Bird
