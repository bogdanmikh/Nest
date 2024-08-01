//
// Created by Admin on 19/03/2022.
//

#include "TriangleLevel.hpp"
#include <Nest.hpp>


class TriangleRenderer final : public Nest::NativeScript {
public:
    TriangleRenderer()
        : m_vertexBuffer()
        , m_indexBuffer()
        , m_shader() {
        using namespace NestRen;

        Nest::ProgramAsset programAsset = Nest::AssetLoader::loadProgram(
            "default-shaders/checker/checker_vertex.glsl",
            "default-shaders/checker/checker_fragment.glsl"
        );
        m_shader = createProgram(programAsset.getNestRenProgramCreate());

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
        NestRen::setShader(m_shader);
        NestRen::setVertexBuffer(m_vertexBuffer);
        NestRen::setIndexBuffer(m_indexBuffer, 0, 6);
        NestRen::submit(0);
    }

private:
    NestRen::VertexBufferHandle m_vertexBuffer;
    NestRen::IndexBufferHandle m_indexBuffer;
    NestRen::ProgramHandle m_shader;
};

void TriangleLevel::start(Nest::World *world) {
    Nest::Entity entity = world->instantiateEntity();
    entity.addNativeScript<TriangleRenderer>();
}
