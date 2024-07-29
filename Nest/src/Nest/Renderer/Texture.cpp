#include <filesystem>
#include "stb_image.h"

#include "Nest/Renderer/Texture.hpp"
#include "Nest/Logger/Logger.hpp"
#include "glad/glad.h"

Texture::Texture(const std::string &path, bool flipVertically) {
    create(path, flipVertically);
}

void Texture::create(const std::string &path, bool flipVertically) {
    if (!std::filesystem::exists(path)) {
        LOG_ERROR("File not exists {}", path);
        return;
    }
    glGenTextures(1, &m_RendererID);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(flipVertically);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format;
        if (nrChannels == 1) {
            format = GL_RED;
            //            std::cout << "R" << std::endl;
        } else if (nrChannels == 3) {
            format = GL_RGB;
            //            std::cout << "RGB" << std::endl;
        } else {
            format = GL_RGBA;
            //            std::cout << "RGBA" << std::endl;
        }

        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
    } else {
        LOG_ERROR("Failed to load texture at path {}", path);
    }
    stbi_image_free(data);
}

void Texture::destroy() {
    if (!m_RendererID) {
        LOG_ERROR("Texture not init");
        return;
    }
    glDeleteTextures(1, &m_RendererID);
    m_RendererID = 0;
}

void Texture::bind(unsigned int slot) const {
    if (!m_RendererID) {
        LOG_ERROR("Texture not init");
        return;
    }
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
