//
// Created by Bogdan on 23.10.2024.
//

#include "NightSky.hpp"

void NightSky::onAttach() {
    std::array<Nest::Path, 6> skyTextureAssetNotBlur = {
        "Textures/NightSky/px.png",
        "Textures/NightSky/nx.png",
        "Textures/NightSky/py.png",
        "Textures/NightSky/ny.png",
        "Textures/NightSky/pz.png",
        "Textures/NightSky/nz.png"
    };
    Nest::Path skyPathToVertexShaderHdr = "Shaders/vstSkyHdr.glsl";
    Nest::Path skyPathToFragmentShaderHdr = "Shaders/fstSkyHdr.glsl";

    Nest::Path skyPathToVertexShaderCubemap = "Shaders/vstSkyCubemap.glsl";
    Nest::Path skyPathToFragmentShaderCubemap = "Shaders/fstSkyCubemap.glsl";

    Nest::Path pathToHdr = "Textures/Hdr/field.jpeg";
    m_nightSky = F_NEW(Foundation::getAllocator(), Nest::SkyComponent)(
        {skyTextureAssetNotBlur, skyPathToVertexShaderCubemap, skyPathToFragmentShaderCubemap}
    );
    //    m_nightSky = F_NEW(Foundation::getAllocator(), Nest::SkyComponent)(
    //        {pathToHdr, skyPathToVertexShaderHdr, skyPathToFragmentShaderHdr}
    //    );
}

void NightSky::onUpdate(double deltaTime) {
    static auto camera = Nest::Application::get()->getWorldCamera();
    projViewMtx = camera->getProjectionMatrix() * camera->getSkyViewMatrix();
    m_nightSky->update(projViewMtx);
}

void NightSky::onImGuiRender() {}

void NightSky::onDetach() {
    F_DELETE(Foundation::getAllocator(), m_nightSky);
}

Nest::TransformComponent &NightSky::getTransform() {}