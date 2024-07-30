#pragma once

#include "Nest/Base/Base.hpp"

#include <string>

class Texture {
public:
    Texture() = default;
    Texture(const std::string &path, bool flipVertically = false);
    void createFromData(int width, int height, int channels, unsigned char *data);
    void create(const std::string &path, bool flipVertically = false);
    void destroy();

    void bind(unsigned int slot = 0) const;
    void unbind() const;
    inline unsigned int getRendererId() const {
        return m_rendererID;
    }
    inline Vec2 getSize() {
        return m_size;
    }
private:
    unsigned int m_rendererID = 0;
    Vec2 m_size = {0, 0};
};
