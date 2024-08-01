//
// Created by Admin on 19/03/2022.
//

#include "TriangleLevel.hpp"

#include <Panda/GameLogic/Components/BaseComponents.hpp>

class TriangleRenderer final : public Panda::NativeScript {
public:
    TriangleRenderer()
        : m_vertexBuffer()
        , m_indexBuffer()
        , m_shader() {
        using namespace Miren;

        Panda::ProgramAsset programAsset = Panda::AssetLoader::loadProgram(
            "default-shaders/checker/checker_vertex.glsl",
            "default-shaders/checker/checker_fragment.glsl"
        );
        m_shader = createProgram(programAsset.getMirenProgramCreate());

        float rightEdge = 0.5f;
        float topEdge = 0.5f;
        float leftEdge = -0.5f;
        float bottomEdge = -0.5f;
        float *data = new float[8]{
            rightEdge, topEdge, leftEdge, topEdge, leftEdge, bottomEdge, rightEdge, bottomEdge
        };
        uint32_t *indices = new uint32_t[6]{0, 1, 2, 0, 2, 3};

        VertexBufferLayoutData layoutData;
        layoutData.pushFloat(2);
        VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);
        m_vertexBuffer = createVertexBuffer(data, sizeof(float) * 8, vertexLayout);
        m_indexBuffer = createIndexBuffer(indices, BufferElementType::UnsignedInt, 6);
    }

    void initialize() override {}

    void update(double deltaTime) override {
        Miren::setShader(m_shader);
        Miren::setVertexBuffer(m_vertexBuffer);
        Miren::setIndexBuffer(m_indexBuffer, 0, 6);
        Miren::submit(0);
    }

private:
    Miren::VertexBufferHandle m_vertexBuffer;
    Miren::IndexBufferHandle m_indexBuffer;
    Miren::ProgramHandle m_shader;
};

void TriangleLevel::start(Panda::World *world) {
    Panda::Entity entity = world->instantiateEntity();
    entity.addNativeScript<TriangleRenderer>();
}
