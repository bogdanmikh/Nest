#include <filesystem>
#include "stb_image.h"

#include "Nest/Renderer/CubeMap.hpp"
#include "Nest/Logger/Logger.hpp"
#include "glad/glad.h"

CubeMap::CubeMap(const std::array<std::string, 6> &paths) {
    create(paths);
}

void CubeMap::create(const std::array<std::string, 6> &paths) {
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
    //    stbi_set_flip_vertically_on_load(true);
    unsigned char *data;
    int width, height, nrChannels;
    for (int i = 0; i < 6; ++i) {
        if (!std::filesystem::exists(paths[i])) {
            LOG_ERROR("File not exists {}", paths[i]);
            return;
        }
        data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGBA,
                width,
                height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                data
            );
        } else {
            LOG_ERROR("Failed to load CubeMap at path {}", paths[i]);
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubeMap::destroy() {
    if (!m_RendererID)
        return;
    glDeleteTextures(1, &m_RendererID);
    m_RendererID = 0;
}

void CubeMap::bind(unsigned int slot) const {
    if (!m_RendererID)
        return;
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
}

void CubeMap::unbind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
