//
// Created by Admin on 11.02.2022.
//

#include "Bird/PlatformData.hpp"
#include "OpenGLTexture.hpp"
#include "Texture/TextureFormat.hpp"

#include "OpenGLBase.hpp"

namespace Bird {

OpenGLTexture::OpenGLTexture()
    : m_id(-1)
    , m_create()
    , m_format(0)
    , m_type(0) {}

void OpenGLTexture::create(TextureCreate &create) {
    NEST_ASSERT(m_id == -1, "TEXTURE ALREADY CREATED");
    m_create = create;
    BIRD_LOG("CREATE TEXTURE, w: {}, h: {}", create.m_width, create.m_height);
    GL_CALL(glGenTextures(1, &m_id));

    GLenum target = create.m_isCubeMap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
    m_target = target;
    GL_CALL(glBindTexture(target, m_id));

    m_format = s_textureFormat[create.m_format].m_fmt;
    GLenum internalFormat = s_textureFormat[create.m_format].m_internalFmt;
    m_type = s_textureFormat[create.m_format].m_type;

    GLenum imageTarget = m_create.m_isCubeMap ? GL_TEXTURE_CUBE_MAP_POSITIVE_X : GL_TEXTURE_2D;
    const uint16_t numSides = create.m_isCubeMap ? 6 : 1;
    size_t imageSize = create.bytesPerTexel() * create.m_width * create.m_height;
    uint8_t *data = (uint8_t *)create.m_data.data;
    for (uint16_t side = 0; side < numSides; ++side) {
        GL_CALL(glTexImage2D(
            imageTarget,
            0,
            m_format,
            create.m_width,
            create.m_height,
            0,
            internalFormat,
            m_type,
            data
        ));
        imageTarget++;
        data += imageSize;
    }
    create.m_data.release();

    if (create.m_numMips > 0) {
        GL_CALL(glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, create.m_numMips));
        GL_CALL(glGenerateMipmap(target));
    }

    switch (create.m_wrap) {
        case CLAMP:
            GL_CALL(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GL_CALL(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GL_CALL(glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
            break;
        case REPEAT:
            GL_CALL(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT));
            GL_CALL(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT));
            GL_CALL(glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_REPEAT));
            break;
    }
    GL_CALL(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, openGLFiltering(create.m_minFiltering)));
    GL_CALL(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, openGLFiltering(create.m_magFiltering)));
}

void OpenGLTexture::resize(uint32_t width, uint32_t height) {
    NEST_ASSERT(m_id != -1, "TEXTURE IS NOT CREATED");
    terminate();
    m_create.m_width = width;
    m_create.m_height = height;
    create(m_create);
}

void OpenGLTexture::terminate() {
    NEST_ASSERT(m_id != -1, "TEXTURE ALREADY DELETED");
    GL_CALL(glDeleteTextures(1, &m_id));
    m_id = -1;
}

void OpenGLTexture::bind(unsigned int slot) {
    NEST_ASSERT(m_id != -1, "TEXTURE IS NOT CREATED");
    GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CALL(glBindTexture(m_target, m_id));
}

void OpenGLTexture::unbind() {
    GL_CALL(glBindTexture(m_target, 0));
}

void OpenGLTexture::readPixels(void *data) {
    GL_CALL(glBindTexture(m_target, m_id));
    //    GL_CALL(glGetTexImage(m_target, 0, m_format, m_type, data));
}

} // namespace Bird
