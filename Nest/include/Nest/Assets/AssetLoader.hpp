#pragma once

#include <NestRen/Base.hpp>

#include <string>

namespace Nest {

struct TextureAsset {
    NestRen::TextureFormat m_format;
    NestRen::TextureWrapMode m_wrap;
    uint16_t m_width;
    uint16_t m_height;
    Foundation::Memory m_data;
    bool m_isCubeMap;

    NestRen::TextureCreate getNestRenTextureCreate() {
        NestRen::TextureCreate create;
        create.m_data = m_data;
        create.m_format = m_format;
        create.m_wrap = m_wrap;
        create.m_width = m_width;
        create.m_height = m_height;
        create.m_isCubeMap = m_isCubeMap;
        return create;
    }
};

struct ProgramAsset {
    Foundation::Memory m_vertex;
    Foundation::Memory m_fragment;

    NestRen::ProgramCreate getNestRenProgramCreate() {
        return {m_vertex, m_fragment};
    }
};

class AssetLoader {
public:
    static TextureAsset loadTexture(const std::string &path);
    static TextureAsset loadCubeMapTexture(std::array<std::string, 6> paths);
    static ProgramAsset loadProgram(const std::string &vertexPath, const std::string &fragmentPath);

    static void setResourcesPath(std::string path) {
        resourcesPath = path + "/";
    }

    static const std::string &getResourcesPath() {
        return resourcesPath;
    }

private:
    static std::string resourcesPath;
};
} // namespace Nest