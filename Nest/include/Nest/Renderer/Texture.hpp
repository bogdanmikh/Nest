#pragma once

#include "Nest/Assets/AssetLoader.hpp"

#include <Bird/Bird.hpp>
#include <cstdlib>

namespace Nest {
class Texture {
public:
    Texture()
        : m_handle(BIRD_INVALID_HANDLE) {}

    Texture(const char *path) {
        TextureAsset asset = AssetLoader::loadTexture(path);
        m_handle = Bird::createTexture(asset.getBirdTextureCreate());
        LOG_INFO("CREATED TEXTURE, path: {}", path);
    }

    Texture(Foundation::Memory mem, uint32_t width, uint32_t height) {
        Bird::TextureCreate create;
        create.m_data = mem;
        create.m_format = Bird::TextureFormat::RGBA8;
        create.m_numMips = 0;
        create.m_width = width;
        create.m_height = height;
        m_handle = Bird::createTexture(create);
        LOG_INFO("CREATED TEXTURE, w: {}, h: {}", width, height);
    }

    Texture(Texture &&other) {
        m_handle = other.m_handle;
        other.m_handle = BIRD_INVALID_HANDLE;
        LOG_INFO("MOVED TEXTURE");
    }

    ~Texture() {
        if (m_handle.isValid()) {
            Bird::deleteTexture(m_handle);
            LOG_INFO("DELETED TEXTURE");
        }
    }

    inline Bird::TextureHandle getHandle() {
        return m_handle;
    }

    bool operator==(const Texture &o) {
        return o.m_handle.id == m_handle.id;
    }

private:
    Bird::TextureHandle m_handle;
};

} // namespace Nest