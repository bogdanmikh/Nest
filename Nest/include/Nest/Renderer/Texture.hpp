#pragma once

#include "Panda/Assets/AssetLoader.hpp"

#include <Miren/Miren.hpp>
#include <cstdlib>

namespace Panda {
class Texture {
public:
    Texture()
        : m_handle(MIREN_INVALID_HANDLE) {}

    Texture(const char *path) {
        TextureAsset asset = AssetLoader::loadTexture(path);
        m_handle = Miren::createTexture(asset.getMirenTextureCreate());
        LOG_INFO("CREATED TEXTURE, path: {}", path);
    }

    Texture(Foundation::Memory mem, uint32_t width, uint32_t height) {
        Miren::TextureCreate create;
        create.m_data = mem;
        create.m_format = Miren::TextureFormat::RGBA8;
        create.m_numMips = 0;
        create.m_width = width;
        create.m_height = height;
        m_handle = Miren::createTexture(create);
        LOG_INFO("CREATED TEXTURE, w: {}, h: {}", width, height);
    }

    Texture(Texture &&other) {
        m_handle = other.m_handle;
        other.m_handle = MIREN_INVALID_HANDLE;
        LOG_INFO("MOVED TEXTURE");
    }

    ~Texture() {
        if (m_handle.isValid()) {
            Miren::deleteTexture(m_handle);
            LOG_INFO("DELETED TEXTURE");
        }
    }

    inline Miren::TextureHandle getHandle() {
        return m_handle;
    }

    bool operator==(const Texture &o) {
        return o.m_handle.id == m_handle.id;
    }

private:
    Miren::TextureHandle m_handle;
};
} // namespace Panda