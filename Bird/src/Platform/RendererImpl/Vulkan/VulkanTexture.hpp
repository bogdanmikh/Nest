//
// Created by Bogdan
//

#pragma once

#include "Bird/Base.hpp"

namespace Bird {

class VulkanTexture {
public:
    VulkanTexture();
    void create(const TextureCreate &create);
    void resize(uint32_t width, uint32_t height);
    uint16_t getWidth() {
        return m_create.m_width;
    }
    uint16_t getHeight() {
        return m_create.m_height;
    }
    void terminate();
    void bind(uint32_t slot);
    void unbind();
    inline uint32_t getId() {
        return m_id;
    }

    inline TextureFormat getFormat() {
        return m_create.m_format;
    }

private:
    TextureCreate m_create;
    uint32_t m_id;
    uint32_t m_target;
};

} // namespace Bird
