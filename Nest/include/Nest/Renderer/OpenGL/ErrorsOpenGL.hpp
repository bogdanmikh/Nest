#pragma once

#include <Nest/Platform/PlatformDetection.hpp>

#ifdef PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PLATFORM_DESKTOP)

#    include <glad/glad.h>
#    include <Nest/Logger/Logger.hpp>
#    include <Nest/Logger/Assert.hpp>
#endif

static const char *getGLErrorStr(GLenum err) {
    switch (err) {
        case GL_NO_ERROR:
            return "No error";
        case GL_INVALID_ENUM:
            return "Invalid enum";
        case GL_INVALID_VALUE:
            return "Invalid value";
        case GL_INVALID_OPERATION:
            return "Invalid operation";
            // case GL_STACK_OVERFLOW:    return "Stack overflow";
            // case GL_STACK_UNDERFLOW:   return "Stack underflow";
        case GL_OUT_OF_MEMORY:
            return "Out of memory";
        default:
            return "Unknown error";
    }
}

#define GL_CALL_INTERNAL(call, assert)                                                             \
    {                                                                                              \
        call;                                                                                      \
        const GLenum err = glGetError();                                                           \
        assert(GL_NO_ERROR == err, #call "; OPENGL ERROR {}", getGLErrorStr(err));                 \
    }

#define GL_CALL(call) GL_CALL_INTERNAL(call, NEST_ASSERT_F)