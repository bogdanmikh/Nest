#include "Nest/Assets/AssetLoader.hpp"

#include <Foundation/Foundation.hpp>
#include <stb_image.h>
#include <fstream>

namespace Nest {

std::string AssetLoader::resourcesPath;

void releaseImage(void *data, void *userInfo) {
    stbi_image_free(data);
}

TextureAsset AssetLoader::loadTexture(const std::string &path) {
    // stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    std::string texturePath = AssetLoader::getResourcesPath() + path;
    void *image = stbi_load(texturePath.c_str(), &width, &height, &channels, 4);

    if (image == nullptr) {
        LOG_ERROR("Failed to load a texture file! {}", stbi_failure_reason());
    }

    TextureAsset texture;
    if (channels == 1) {
        texture.m_format = NestRen::TextureFormat::RED_INTEGER;
    } else if (channels == 3) {
        texture.m_format = NestRen::TextureFormat::RGB8;
    } else {
        texture.m_format = NestRen::TextureFormat::RGBA8;
    }
    texture.m_width = width;
    texture.m_height = height;
    texture.m_data = Foundation::Memory(image);
    texture.m_data.releaseFn = &releaseImage;
    texture.m_wrap = NestRen::TextureWrapMode::REPEAT;
    texture.m_isCubeMap = false;
    return texture;
}

TextureAsset AssetLoader::loadCubeMapTexture(std::array<std::string, 6> paths) {
    TextureAsset texture;
    std::array<void *, 6> images;
    int bytesPerColor;
    for (uint16_t side = 0; side < 6; ++side) {
        const std::string &path = paths[side];
        int width, height, channels;
        std::string texturePath = AssetLoader::getResourcesPath() + path;
        void *image = stbi_load(texturePath.c_str(), &width, &height, &channels, 4);
        if (image == nullptr) {
            LOG_ERROR("Failed to load a texture file at path {}", path);
            continue;
        }
        if (channels == 1) {
            texture.m_format = NestRen::TextureFormat::RED_INTEGER;
            bytesPerColor = 1;
        } else if (channels == 3) {
            texture.m_format = NestRen::TextureFormat::RGB8;
            bytesPerColor = 3;
        } else {
            texture.m_format = NestRen::TextureFormat::RGBA8;
            bytesPerColor = 4;
        }
        texture.m_width = width;
        texture.m_height = height;
        images[side] = image;
    }
    size_t imageSize = bytesPerColor * texture.m_width * texture.m_height;
    texture.m_data = Foundation::Memory::alloc(imageSize * 6);
    uint8_t *ptr = (uint8_t *)texture.m_data.data;
    for (uint16_t side = 0; side < 6; ++side) {
        memcpy(ptr, images[side], imageSize);
        ptr += imageSize;
        stbi_image_free(images[side]);
    }
    texture.m_wrap = NestRen::TextureWrapMode::REPEAT;
    texture.m_isCubeMap = true;
    return texture;
}

ProgramAsset
AssetLoader::loadProgram(const std::string &vertexPath, const std::string &fragmentPath) {
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(AssetLoader::getResourcesPath() + vertexPath);
        fShaderFile.open(AssetLoader::getResourcesPath() + fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure &e) {
        NEST_ASSERT_F(
            false, "SHADER::FILE {} or {} NOT SUCCESSFULLY READ", vertexPath, fragmentPath
        );
    }
    Foundation::Memory vertexData =
        Foundation::Memory::copying((void *)vertexCode.c_str(), vertexCode.size() + 1);
    Foundation::Memory fragmentData =
        Foundation::Memory::copying((void *)fragmentCode.c_str(), fragmentCode.size() + 1);
    return {vertexData, fragmentData};
}

} // namespace Panda