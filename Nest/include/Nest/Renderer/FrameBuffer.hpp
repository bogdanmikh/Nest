#pragma once

#include "Texture.hpp"
#include "Nest/Base/Base.hpp"

class FrameBuffer {
public:
    explicit FrameBuffer(const Vec2 &size);
    ~FrameBuffer();
    void bind() const;
    void unbind() const;
    void resize(const Vec2 &size);
    uint32_t getRendererIdTexture();
    Vec2 getSize();
    Texture *getTexture();

private:
    static void checkStatus();
    unsigned int m_RendererID;
    Texture texture;
};