#pragma once

#include <string>

class Texture {
private:
    unsigned int m_RendererID;

public:
    Texture(const std::string &path);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;
};
