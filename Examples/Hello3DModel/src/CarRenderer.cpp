//
// Created by Bogdan on 06.08.2024.
//

#include "CarRenderer.hpp"

CarRenderer::CarRenderer() {}

void CarRenderer::onAttach() {
    Nest::ProgramAsset programAsset = Nest::AssetLoader::loadProgram(
        "Shaders/Model3d_vertex.glsl", "Shaders/Model3d_fragment.glsl"
    );
    m_model3D.create(
        createProgram(programAsset.getBirdProgramCreate()), "Models/cube.obj"
    );
}

void CarRenderer::onUpdate(double deltaTime) {
    //    Bird::setState(BIRD_STATE_DEPTH_TEST);
    //    m_model3D.getTransform().setScale({1., 1., 1.});
   // m_model3D.draw();
}

void CarRenderer::onImGuiRender() {}

void CarRenderer::onDetach() {}