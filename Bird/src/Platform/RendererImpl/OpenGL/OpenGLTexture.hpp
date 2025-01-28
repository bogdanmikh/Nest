//
// Created by Admin on 11.02.2022.
//

#pragma once

#include "Bird/Base.hpp"

namespace Bird {

class OpenGLTexture {
public:
    OpenGLTexture();
    void create(TextureCreate &create);
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

    void readPixels(void *data);

private:
    TextureCreate m_create;
    uint32_t m_id;
    uint32_t m_target;
    uint32_t m_format;
    uint32_t m_type;
};

} // namespace Bird
