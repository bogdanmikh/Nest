//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Bird/VertexBufferLayoutData.hpp"
#include "Bird/Base.hpp"
#include "VulkanBuffer.hpp"

namespace Bird {

class VulkanVertexBuffer {
public:
    VulkanVertexBuffer();
    void create(void *data, uint32_t size, bool isDynamic);
    void update(void *data, uint32_t size);
    void terminate();
    void bind();
    void setLayoutHandle(VertexLayoutHandle layoutHandle);
    VertexLayoutHandle getLayoutHandle();

    VulkanBufferDelegate *m_delegate;

private:
    bool m_isDynamic;
    VertexLayoutHandle m_layoutHandle;
    VulkanBuffer m_buffer;
};

} // namespace Bird