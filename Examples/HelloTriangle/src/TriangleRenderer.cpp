#include "TriangleRenderer.hpp"

TriangleRenderer::TriangleRenderer() {}

void TriangleRenderer::onAttach() {
    using namespace Bird;
    Nest::Path vertexShaderPath, fragmentShaderPath;

    //    vertexShaderPath = "Shaders/vst.glsl";
    //    fragmentShaderPath = "Shaders/fst.glsl";
    //    Nest::ProgramAsset programAsset =
    //        Nest::AssetLoader::loadProgram(vertexShaderPath, fragmentShaderPath);
    vertexShaderPath = "Shaders/vst.spv";
    fragmentShaderPath = "Shaders/fst.spv";
    Nest::ProgramAsset programAsset =
        Nest::AssetLoader::loadProgramBin(vertexShaderPath, fragmentShaderPath);
    m_shader = createProgram(programAsset.getBirdProgramCreate());

    VertexTriangle vertices[3] = {
        VertexTriangle(0.0, -0.5, 0.0),
        VertexTriangle(0.5, 0.5, 0.0),
        VertexTriangle(-0.5, 0.5, 0.0)
    };

    uint32_t indices[3]{0, 1, 2};
    Foundation::Memory indicesMemory = Foundation::Memory::copying(indices, sizeof(uint32_t) * 3);

    Foundation::Memory verticesMemory =
        Foundation::Memory::copying(vertices, sizeof(VertexTriangle) * 24);
    VertexBufferLayoutData layoutData;
    layoutData.pushVec3();
    VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);
    m_vertexBuffer = createVertexBuffer(verticesMemory, 24 * sizeof(VertexTriangle), vertexLayout);
    m_indexBuffer = createIndexBuffer(indicesMemory, BufferElementType::UnsignedInt, 3);
}

void TriangleRenderer::onUpdate(double deltaTime) {
    Bird::setShader(m_shader);
    Bird::setIndexBuffer(m_indexBuffer, 0, 3);
    Bird::setVertexBuffer(m_vertexBuffer);
    Bird::submit(0);
}

void TriangleRenderer::onImGuiRender() {}

void TriangleRenderer::onDetach() {
    deleteProgram(m_shader);
}

Nest::TransformComponent &TriangleRenderer::getTransform() {
    return m_transformComponent;
}