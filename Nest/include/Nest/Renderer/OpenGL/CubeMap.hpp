#pragma once

#include <string>

class CubeMap {
private:
    unsigned int m_RendererID = 0;

public:
    CubeMap() = default;
    CubeMap(const std::array<std::string, 6> &paths);
    void create(const std::array<std::string, 6> &paths);
    void destroy();

    void bind(unsigned int slot = 0) const;
    void unbind() const;
    unsigned int getRendererId() const {
        return m_RendererID;
    }
};
