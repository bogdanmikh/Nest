#include "NestPBR.hpp"
#include <imgui.h>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

void NestPBR::onAttach() {
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
    m_cubeRenderer.onAttachFigure(cubeCreateInfo);

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
    m_spheres[0].onAttachFigure(sphereCreateInfo);

    sphereCreateInfo.position = glm::vec3(6., 0., 0.);
    sphereCreateInfo.pathToTexture = "Textures/Rust.jpg";
    m_spheres[1].onAttachFigure(sphereCreateInfo);

    sphereCreateInfo.position = glm::vec3(8., 0., 0.);
    sphereCreateInfo.pathToTexture = "Textures/Scratch.jpg";
    m_spheres[2].onAttachFigure(sphereCreateInfo);

    m_cameraMove.onAttach();
}

void NestPBR::onDetach() {
    m_cameraMove.onDetach();
    m_cubeRenderer.onDetach();
    for (auto &sphere : m_spheres) {
        sphere.onDetach();
    }
}

void NestPBR::onUpdate(double deltaTime) {
    m_cameraMove.onUpdate(deltaTime);
    auto rotationCube = m_cubeRenderer.getTransform().getRotationEuler();
    rotationCube.x += 0.5;
    rotationCube.y += 0.5;
    m_cubeRenderer.getTransform().setRotationEuler(rotationCube);
    m_cubeRenderer.onUpdate(deltaTime);

    // spheres
//    for (auto &sphere : m_spheres) {
//        sphere.onUpdate(deltaTime);
//    }
}

void NestPBR::addEntity(Nest::Entity *entity) {}

NestPBR::~NestPBR() {}
