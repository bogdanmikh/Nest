#pragma once

#include <Foundation/Assert.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PLATFORM_ANDROID)
#    include <GLES3/gl3.h>
#elif defined(PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Bird {

const char *getGLErrorStr(GLenum err);

#define GL_CALL_INTERNAL(call, assert)                                                             \
    {                                                                                              \
        call;                                                                                      \
        const GLenum err = glGetError();                                                           \
        assert(GL_NO_ERROR == err, #call "; OPENGL ERROR {}", getGLErrorStr(err));                 \
    }

#define GL_CALL(call) GL_CALL_INTERNAL(call, NEST_ASSERT_F)

} // namespace Bird