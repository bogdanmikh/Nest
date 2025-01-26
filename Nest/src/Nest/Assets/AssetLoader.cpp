#include "Nest/Assets/AssetLoader.hpp"
#include "Nest/Assets/ShaderEnvelope.hpp"

#include <Foundation/Foundation.hpp>
#include <fstream>
#include <stb_image.h>
#include <Foundation/PlatformDetection.hpp>

#ifdef PLATFORM_ANDROID
#    include <android/asset_manager.h>
#    include <android/asset_manager_jni.h>
#    include <Bird/PlatformData.hpp>
#endif

namespace Nest {

std::string AssetLoader::resourcesPath;

void releaseImage(void *data, void *userInfo) {
    stbi_image_free(data);
}

std::optional<std::pair<Foundation::Memory, int>> AssetLoader::readFile(const std::string &path) {
#ifdef PLATFORM_ANDROID
    AAsset *asset = AAssetManager_open(
        (AAssetManager *)Bird::PlatformData::get()->assetManager, path.c_str(), AASSET_MODE_RANDOM
    );
    std::pair<Foundation::Memory, int> res;
    bool assetLoaded = false;
    if (asset) {
        off_t fileLength = AAsset_getLength(asset);

        void *buffer = malloc(fileLength);
        AAsset_read(asset, buffer, fileLength);

        res.first = Foundation::Memory::copying(buffer, fileLength);
        res.second = fileLength;
        assetLoaded = true;

        free(buffer);
        AAsset_close(asset);
    }
    if (assetLoaded) {
        return res;
    }
    return {};
#endif
    return {};
}

TextureAsset AssetLoader::loadTexture(const std::string &path) {
    int width, height, channels;
    std::string texturePath = AssetLoader::getResourcesPath() + path;
    void *image = nullptr;
#ifdef PLATFORM_DESKTOP
    image = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);
#elif defined(PLATFORM_ANDROID)
    auto textureData = readFile(texturePath);

    if (!textureData.has_value()) {
        LOG_ERROR("Failed to load a texture file!");
        return {};
    }
    image = stbi_load_from_memory(
        (unsigned char *)textureData.value().first.data,
        textureData.value().second,
        &width,
        &height,
        &channels,
        0
    );
#endif
    // stbi_set_flip_vertically_on_load(true);

    if (image == nullptr) {
        LOG_ERROR("Failed to load a texture file! {}", stbi_failure_reason());
    }

    TextureAsset texture;
    if (channels == 1) {
        texture.m_format = Bird::TextureFormat::RED_INTEGER;
    } else if (channels == 3) {
        texture.m_format = Bird::TextureFormat::RGB8;
    } else {
        texture.m_format = Bird::TextureFormat::RGBA8;
    }
    texture.m_width = width;
    texture.m_height = height;
    texture.m_data = Foundation::Memory(image);
    texture.m_data.releaseFn = &releaseImage;
    texture.m_wrap = Bird::TextureWrapMode::REPEAT;
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
            texture.m_format = Bird::TextureFormat::RED_INTEGER;
            bytesPerColor = 1;
        } else if (channels == 3) {
            texture.m_format = Bird::TextureFormat::RGB8;
            bytesPerColor = 3;
        } else {
            texture.m_format = Bird::TextureFormat::RGBA8;
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
    texture.m_wrap = Bird::TextureWrapMode::REPEAT;
    texture.m_isCubeMap = true;
    return texture;
}

ProgramAsset
AssetLoader::loadProgram(const std::string &vertexPath, const std::string &fragmentPath) {
#ifdef PLATFORM_DESKTOP
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
#elif defined(PLATFORM_ANDROID)
    auto vertexMemory = readFile(vertexPath);
    if (!vertexMemory.has_value()) {
        NEST_ASSERT_F(
            false, "SHADER::FILE {} or {} NOT SUCCESSFULLY READ", vertexPath, fragmentPath
        );
    }
    auto fragmentMemory = readFile(fragmentPath);
    if (!fragmentMemory.has_value()) {
        NEST_ASSERT_F(
            false, "SHADER::FILE {} or {} NOT SUCCESSFULLY READ", vertexPath, fragmentPath
        );
    }
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << (const char *)vertexMemory.value().first.data;
    fShaderStream << (const char *)fragmentMemory.value().first.data;

    auto vertexCode = vShaderStream.str();
    auto fragmentCode = fShaderStream.str();

    Foundation::Memory vertexData =
        Foundation::Memory::copying((void *)vertexCode.c_str(), vertexCode.size() + 1);
    Foundation::Memory fragmentData =
        Foundation::Memory::copying((void *)fragmentCode.c_str(), fragmentCode.size() + 1);
    return {vertexData, fragmentData};
#endif
}

ProgramAsset AssetLoader::loadProgramXml(const std::string &pathXml) {
    auto shadersCode = ShaderEnvelope::openEnvelope(pathXml);

    std::string vertexCode, fragmentCode;
    auto shaders = shadersCode.shadersData;
    if (shaders[ShaderEnvelope::VERTEX].code.has_value()) {
        vertexCode = shaders[ShaderEnvelope::VERTEX].code.value();
    } else {
        LOG_ERROR("VERTEX SHADER CODE NOT FOUND");
    }

    if (shaders[ShaderEnvelope::FRAGMENT].code.has_value()) {
        fragmentCode = shaders[ShaderEnvelope::FRAGMENT].code.value();
    } else {
        LOG_ERROR("VERTEX SHADER CODE NOT FOUND");
    }

    Foundation::Memory vertexData =
        Foundation::Memory::copying((void *)vertexCode.c_str(), vertexCode.size() + 1);
    Foundation::Memory fragmentData =
        Foundation::Memory::copying((void *)fragmentCode.c_str(), fragmentCode.size() + 1);
    return {vertexData, fragmentData};
}

} // namespace Nest