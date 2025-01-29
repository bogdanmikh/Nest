#pragma once

#include <Foundation/PlatformDetection.hpp>
#include <Foundation/Foundation.hpp>

#ifdef PLATFORM_IOS
#    include <OpenGLES/ES3/gl.h>
#elif defined(PLATFORM_ANDROID)
#    include <GLES3/gl3.h>
#elif defined(PLATFORM_DESKTOP)
#    include <glad/glad.h>
#endif

namespace Bird {

class OpenGLExtensions {
public:
    struct Extension {
        enum Enum {
            EXT_bgra,
            EXT_blend_color,
            EXT_blend_minmax,
            EXT_blend_subtract,

            EXT_COUNT
        };

        const char *m_name;
        bool m_isSupported;
    };

    static void initialize() {
        shared().initializeExtensions();
    }

    static bool isSupported(Extension::Enum ext) {
        return shared().m_extensions[ext].m_isSupported;
    }

    static OpenGLExtensions &shared() {
        static OpenGLExtensions instance;
        return instance;
    }

private:
    OpenGLExtensions() {}

    void initializeExtensions() {
        std::vector<std::string> supportedExtensions = getSupportedExtensions();
        for (int i = 0; i < Extension::Enum::EXT_COUNT; ++i) {
            Extension &extension = m_extensions[i];
            extension.m_isSupported = isExtensionSupported(extension.m_name, supportedExtensions);
            BIRD_LOG(
                "GL_EXTENSION {}: {}",
                extension.m_name,
                extension.m_isSupported ? "(supported)" : "(not)"
            );
        }
    }

    bool isExtensionSupported(
        const char *extension, const std::vector<std::string> &supportedExtensions
    ) {
        for (const auto &ext : supportedExtensions) {
            if (strcmp(extension, ext.c_str()) == 0) {
                return true;
            }
        }
        return false;
    }

    std::vector<std::string> getSupportedExtensions() {
        std::vector<std::string> extensions;

        GLint numExtensions = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

        for (GLint i = 0; i < numExtensions; ++i) {
            const char *ext = reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, i));
            if (ext) {
                if (Foundation::strCmp(ext, "GL_", 3) == 0) {
                    ext += 3;
                }
                extensions.push_back(ext);
            }
        }

        return extensions;
    }

    Extension m_extensions[Extension::EXT_COUNT] = {
        {"EXT_bgra", false},
        {"EXT_blend_color", false},
        {"EXT_blend_minmax", false},
        {"EXT_blend_subtract", false},
    };
};

} // namespace Bird