#pragma once

#include <filesystem>
#include <array>
#include <glm/glm.hpp>

struct CreateInfo {
    bool useTexture = false;
    bool useCubeMap = false;
    glm::vec3 position;
    std::filesystem::path pathToTexture;
    std::array<std::string, 6> skyTextureAsset;
    std::filesystem::path pathToVertexShader;
    std::filesystem::path pathToFragmentShader;
    std::filesystem::path pathToGeometryShader;
    std::string nameTexture;
    std::string nameCubeMap;
};

struct Figure {
    virtual void init(const CreateInfo &createInfo) = 0;
    virtual void draw(double deltaTime) = 0;
};