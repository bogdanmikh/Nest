#pragma once

#include "Nest/Assets/AssetLoader.hpp"

#include <NestRen/NestRen.hpp>
#include <cstdlib>

namespace Nest {
class Texture {
public:
    Texture()
        : m_handle(NESTREN_INVALID_HANDLE) {}

    Texture(const char *path) {
        TextureAsset asset = AssetLoader::loadTexture(path);
        m_handle = NestRen::createTexture(asset.getNestRenTextureCreate());
        LOG_INFO("CREATED TEXTURE, path: {}", path);
    }

    Texture(Foundation::Memory mem, uint32_t width, uint32_t height) {
        NestRen::TextureCreate create;
        create.m_data = mem;
        create.m_format = NestRen::TextureFormat::RGBA8;
        create.m_numMips = 0;
        create.m_width = width;
        create.m_height = height;
        m_handle = NestRen::createTexture(create);
        LOG_INFO("CREATED TEXTURE, w: {}, h: {}", width, height);
    }

    Texture(Texture &&other) {
        m_handle = other.m_handle;
        other.m_handle = NESTREN_INVALID_HANDLE;
        LOG_INFO("MOVED TEXTURE");
    }

    ~Texture() {
        if (m_handle.isValid()) {
            NestRen::deleteTexture(m_handle);
            LOG_INFO("DELETED TEXTURE");
        }
    }

    inline NestRen::TextureHandle getHandle() {
        return m_handle;
    }

    bool operator==(const Texture &o) {
        return o.m_handle.id == m_handle.id;
    }

private:
    NestRen::TextureHandle m_handle;
};
} // namespace Nest