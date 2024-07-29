#pragma once

#include <string>

class Texture {
private:
    unsigned int m_RendererID = 0;
public:
    Texture() = default;
    Texture(const std::string &path, bool flipVertically = false);
    void create(const std::string &path, bool flipVertically = false);
    void destroy();

    void bind(unsigned int slot = 0) const;
    void unbind() const;
    unsigned int getRendererId() const {
        return m_RendererID;
    }
};
