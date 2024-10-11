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
        createProgram(programAsset.getBirdProgramCreate()), "Models/tank.glb"
    );
}

void CarRenderer::onUpdate(double deltaTime) {
  // m_model3D.getTransform().setScale({1., 1., 1.});
    m_model3D.getTransform().setPosition(glm::vec3(0, 0, 0));
    m_model3D.draw();
}

void CarRenderer::onImGuiRender() {}

void CarRenderer::onDetach() {}