//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "NestRen/VertexBufferLayoutData.hpp"
#include "NestRen/Base.hpp"

namespace NestRen {

class OpenGLVertexBuffer {
public:
    OpenGLVertexBuffer();
    void create(void *data, uint32_t size, bool isDynamic);
    void update(void *data, uint32_t size);
    void terminate();
    void bind();
    void unbind();
    void setLayoutHandle(VertexLayoutHandle layoutHandle);
    VertexLayoutHandle getLayoutHandle();

private:
    uint32_t m_id;
    bool m_isDynamic;
    VertexLayoutHandle m_layoutHandle;
};

} // namespace NestRen