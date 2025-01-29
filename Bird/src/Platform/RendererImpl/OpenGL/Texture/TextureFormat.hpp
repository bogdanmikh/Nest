#pragma once

#include "Bird/Base.hpp"

#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PLATFORM_ANDROID)
#    include <GLES3/gl3.h>
#elif defined(PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Bird {

struct TextureFormatInfo {
    GLenum m_fmt;
    GLenum m_internalFmt;
    GLenum m_type;
};

const static TextureFormatInfo s_textureFormat[] = {
    {GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE},   // RGB8
    {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE}, // RGBA8
    {GL_R32I, GL_RED_INTEGER, GL_INT},     // RED_INTEGER
    // Depth/stencil
    {GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8} // DEPTH24STENCIL8
};

static GLint openGLFiltering(TextureFiltering filtering) {
    switch (filtering) {
        case NEAREST:
            return GL_NEAREST;
        case LINEAR:
            return GL_LINEAR;
        case NEAREST_MIPMAP_NEAREST:
            return GL_NEAREST_MIPMAP_NEAREST;
        case NEAREST_MIPMAP_LINEAR:
            return GL_NEAREST_MIPMAP_LINEAR;
        case LINEAR_MIPMAP_NEAREST:
            return GL_LINEAR_MIPMAP_NEAREST;
        case LINEAR_MIPMAP_LINEAR:
            return GL_LINEAR_MIPMAP_LINEAR;
    }
}

} // namespace Bird