#include "NestPBR.hpp"
#include <imgui.h>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

void ProfilerTest::start() {
    std::array<std::string, 6> skyTextureAssetNotBlur = {
        "Textures/skybox/notblur/px.png",
        "Textures/skybox/notblur/nx.png",
        "Textures/skybox/notblur/py.png",
        "Textures/skybox/notblur/ny.png",
        "Textures/skybox/notblur/pz.png",
        "Textures/skybox/notblur/nz.png"
    };
    std::array<std::string, 6> skyTextureAssetBlur = {
        "Textures/skybox/blur/px.png",
        "Textures/skybox/blur/nx.png",
        "Textures/skybox/blur/py.png",
        "Textures/skybox/blur/ny.png",
        "Textures/skybox/blur/pz.png",
        "Textures/skybox/blur/nz.png"
    };
    m_skyComponent.init(skyTextureAssetNotBlur);

    CreateInfo cubeCreateInfo;
    cubeCreateInfo.position = glm::vec3(0., 0., 0.);
    cubeCreateInfo.pathToVertexShader = "Shaders/vstCube.glsl";
    cubeCreateInfo.pathToFragmentShader = "Shaders/fstCube.glsl";
//    cubeCreateInfo.pathToGeometryShader = "Shaders/gstCube.glsl";
    cubeCreateInfo.useTexture = true;
    cubeCreateInfo.pathToTexture = "Textures/Rust.jpg";
    cubeCreateInfo.useCubeMap = true;
    cubeCreateInfo.skyTextureAsset = skyTextureAssetNotBlur;
    cubeCreateInfo.nameTexture = "iTexture";
    cubeCreateInfo.nameCubeMap = "iSky";
    m_cube.init(cubeCreateInfo);

    CreateInfo sphereCreateInfo;
    sphereCreateInfo.position = glm::vec3(3., 0., 0.);
    sphereCreateInfo.pathToVertexShader = "Shaders/vstSphere.glsl";
    sphereCreateInfo.pathToFragmentShader = "Shaders/fstSphere.glsl";
    sphereCreateInfo.useTexture = true;
    sphereCreateInfo.pathToTexture = "Textures/Sphere.jpg";
    sphereCreateInfo.useCubeMap = true;
    sphereCreateInfo.skyTextureAsset = skyTextureAssetNotBlur;
    sphereCreateInfo.nameTexture = "iTexture";
    sphereCreateInfo.nameCubeMap = "iSky";
    m_spheres.resize(3);
    m_spheres[0].init(sphereCreateInfo);

    sphereCreateInfo.position = glm::vec3(6., 0., 0.);
    sphereCreateInfo.pathToTexture = "Textures/Rust.jpg";
    m_spheres[1].init(sphereCreateInfo);

    sphereCreateInfo.position = glm::vec3(8., 0., 0.);
    sphereCreateInfo.pathToTexture = "Textures/Scratch.jpg";
    m_spheres[2].init(sphereCreateInfo);

    m_cameraMove.init();
}

void ProfilerTest::detach() {}

void ProfilerTest::update(double deltaTime) {
    m_cameraMove.update(deltaTime);
    m_skyComponent.draw();
    m_cube.rotateZ(0.5);
    m_cube.rotateY(0.5);
    m_cube.draw(deltaTime);

    // spheres
    Sphere::drawSettings();
    float speed = 0.00001;
    for (auto &sphere : m_spheres) {
        sphere.draw(deltaTime);
    }
    Renderer::checkForErrors();
}