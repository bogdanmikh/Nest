#include "NestPBR.hpp"
#include <filesystem>
#include <imgui.h>
#include <iostream>
#include <sstream>
#include <string>

void NestPBR::onAttach() {
    m_cameraMove.onAttach();

    std::array<Nest::Path, 6> skyTextureAssetNotBlur = {
        "Textures/skybox/notblur/px.png",
        "Textures/skybox/notblur/nx.png",
        "Textures/skybox/notblur/py.png",
        "Textures/skybox/notblur/ny.png",
        "Textures/skybox/notblur/pz.png",
        "Textures/skybox/notblur/nz.png"
    };
    Nest::Path skyPathToVertexShader = "Shaders/vstSky.glsl";
    Nest::Path skyPathToFragmentShader = "Shaders/fstSky.glsl";
    m_skyComponent = F_NEW(Foundation::getAllocator(), Nest::SkyComponent)(
        {skyTextureAssetNotBlur, skyPathToVertexShader, skyPathToFragmentShader}
    );

    //    std::array<std::string, 6> skyTextureAssetBlur = {
    //        "Textures/skybox/blur/px.png",
    //        "Textures/skybox/blur/nx.png",
    //        "Textures/skybox/blur/py.png",
    //        "Textures/skybox/blur/ny.png",
    //        "Textures/skybox/blur/pz.png",
    //        "Textures/skybox/blur/nz.png"
    //    };

    CreateInfo cubeCreateInfo;
    cubeCreateInfo.position = glm::vec3(0., 0., 0.);
    cubeCreateInfo.pathToVertexShader = "Shaders/vstCube.glsl";
    cubeCreateInfo.pathToFragmentShader = "Shaders/fstCube.glsl";
    cubeCreateInfo.useTexture = true;
    cubeCreateInfo.pathToTexture = "Textures/Rust.jpg";
    cubeCreateInfo.useCubeMap = true;
    cubeCreateInfo.skyComponentHandle = m_skyComponent->getSkyTexture();
    cubeCreateInfo.nameTexture = "iTexture";
    cubeCreateInfo.nameSkyTexture = "iSky";
    m_cubeRenderer.onAttachFigure(cubeCreateInfo);

    m_spheres.resize(3);

    CreateInfo sphereCreateInfo;
    sphereCreateInfo.pathToVertexShader = "Shaders/vstSphere.glsl";
    sphereCreateInfo.pathToFragmentShader = "Shaders/fstSphere.glsl";
    sphereCreateInfo.useTexture = true;
    sphereCreateInfo.pathToTexture = "Textures/Sphere.jpg";
    sphereCreateInfo.useCubeMap = true;
    sphereCreateInfo.nameTexture = "iTexture";
    sphereCreateInfo.skyComponentHandle = m_skyComponent->getSkyTexture();
    sphereCreateInfo.nameSkyTexture = "iSky";
    sphereCreateInfo.position = glm::vec3(3., 0., 0.);
    m_spheres[0].onAttachFigure(sphereCreateInfo);

    sphereCreateInfo.position = glm::vec3(6., 0., 0.);
    sphereCreateInfo.pathToTexture = "Textures/Dubil.png";
    m_spheres[1].onAttachFigure(sphereCreateInfo);

    sphereCreateInfo.position = glm::vec3(9., 0., 0.);
    sphereCreateInfo.pathToTexture = "Textures/Rust.jpg";
    m_spheres[2].onAttachFigure(sphereCreateInfo);
}

void NestPBR::onUpdate(double deltaTime) {
    m_cameraMove.onUpdate(deltaTime);
    m_cameraMove.onImGuiRender();

    static auto camera = Nest::Application::get()->getWorldCamera();
    static glm::mat4 projViewMtx;
    projViewMtx = camera->getProjectionMatrix() * camera->getSkyViewMatrix();
    m_skyComponent->update(projViewMtx);

    auto rotationCube = m_cubeRenderer.getTransform().getRotationEuler();
    rotationCube.x += 0.5;
    rotationCube.y += 0.5;
    m_cubeRenderer.getTransform().setRotationEuler(rotationCube);
    m_cubeRenderer.onUpdate(deltaTime);

    auto rotationSphere = m_spheres[0].getTransform().getRotationEuler();
    rotationSphere.x += 0.9;
    m_spheres[0].getTransform().setRotationEuler(rotationSphere);

    rotationSphere = m_spheres[1].getTransform().getRotationEuler();
    rotationSphere.z = 90;
    rotationSphere.y += 3;
    m_spheres[1].getTransform().setRotationEuler(rotationSphere);

    // spheres
    for (auto &sphere : m_spheres) {
        sphere.onUpdate(deltaTime);
    }
}

void NestPBR::onDetach() {
    m_cameraMove.onDetach();
    m_cubeRenderer.onDetach();
    for (auto &sphere : m_spheres) {
        sphere.onDetach();
    }
    F_DELETE(Foundation::getAllocator(), m_skyComponent);
}

void NestPBR::addEntity(Nest::Entity *entity) {}

NestPBR::~NestPBR() {}
