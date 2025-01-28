//
// Created by Bogdan on 23.10.2024.
//

#include "NightSky.hpp"

void NightSky::onAttach() {
    //    std::array<Nest::Path, 6> skyTextureAssetNotBlur = {
    //        "Textures/NightSky/px.png",
    //        "Textures/NightSky/nx.png",
    //        "Textures/NightSky/py.png",
    //        "Textures/NightSky/ny.png",
    //        "Textures/NightSky/pz.png",
    //        "Textures/NightSky/nz.png"
    //    };
    Nest::Path skyPathToVertexShader = "Shaders/vstSky.glsl";
    Nest::Path skyPathToFragmentShader = "Shaders/fstSky.glsl";
    m_nightSky = F_NEW(Foundation::getAllocator(), Nest::SkyComponent)(
        {"Textures/Hdr/field.jpeg", skyPathToVertexShader, skyPathToFragmentShader}
    );
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