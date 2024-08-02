#pragma once

#include <Nest.hpp>

class TriangleRenderer final : public Nest::Entity {
public:
    void onAttach() {
        using namespace NestRen;

        NestRen::setViewClear(0, 0x3D75C9FF);

        Nest::ProgramAsset programAsset = Nest::AssetLoader::loadProgram(
            "default-shaders/checker/checker_vertex.glsl",
            "default-shaders/checker/checker_fragment.glsl"
        );
        m_shader = createProgram(programAsset.getNestRenProgramCreate());

        float rightEdge = 0.5f;
        float topEdge = 0.5f;
        float leftEdge = -0.5f;
        float bottomEdge = -0.5f;
        auto *data = new float[8]{
            rightEdge, topEdge, leftEdge, topEdge, leftEdge, bottomEdge, rightEdge, bottomEdge
        };
        auto *indices = new uint32_t[6]{0, 1, 2, 0, 2, 3};

        VertexBufferLayoutData layoutData;
        layoutData.pushFloat(2);
        VertexLayoutHandle vertexLayout = createVertexLayout(layoutData);
        m_vertexBuffer = createVertexBuffer(data, sizeof(float) * 8, vertexLayout);
        m_indexBuffer = createIndexBuffer(indices, BufferElementType::UnsignedInt, 6);
        //        delete[] data;
        //        delete[] indices;
    }

    void onUpdate(double deltaTime) override {
        NestRen::setShader(m_shader);
        static float time = Nest::Application::get()->getWindow()->getTime();
        NestRen::setUniform(m_shader, "iTime", &time, NestRen::UniformType::Vec4);
        NestRen::setVertexBuffer(m_vertexBuffer);
        NestRen::setIndexBuffer(m_indexBuffer, 0, 6);
        NestRen::submit(0);
    }

    void onImGuiRender() override {
        ImGui::SetNextWindowPos({100, 100});
        ImGui::SetNextWindowSize({100, 100});
        LOG_INFO("ImGUi renderer");
        ImGui::Begin("Loshara");
        ImGui::Text("Ya loh");
        ImGui::End();
    }

    void onDetach() override {
        deleteVertexBuffer(m_vertexBuffer);
        deleteIndexBuffer(m_indexBuffer);
        deleteProgram(m_shader);
    }

private:
    NestRen::VertexBufferHandle m_vertexBuffer;
    NestRen::IndexBufferHandle m_indexBuffer;
    NestRen::ProgramHandle m_shader;
};
