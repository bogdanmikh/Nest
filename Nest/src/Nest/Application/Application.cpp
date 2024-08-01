#include <chrono>

#include "imgui.h"
#include "Nest/Application/Application.hpp"
#include "Nest/Window/Events.hpp"

#include <NestRen/NestRen.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/Allocator.hpp>

namespace Nest {

Application *Application::s_instance = nullptr;

uint64_t getMillis() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );
    return now.count();
}


Application::Application(ApplicationStartupSettings &settings) {
    s_instance = this;
    NestRen::initialize();
//    m_ImGuiLayer = NEW(Foundation::getAllocator(), ImGuiLayer);
//    m_ImGuiLayer->onAttach();

    m_currentLevel = nullptr;
    Foundation::Logger::init();
    m_window = new Window;
    m_window->init(settings.name, settings.windowSize.x, settings.windowSize.y, settings.isFullScreen);

    timeMillis = getMillis();
    m_lastViewportSize = m_window->getSize();
}

Application::~Application() {
//    m_ImGuiLayer->onDetach();
    delete m_window;
}


//void Application::drawProperties() const {
//    ImGui::Begin("Stats");
//    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
//    ImGui::Text("FPS: %d", fps);
//    ImGui::PopStyleColor();
//    ImGui::PopStyleColor();
//    ImGui::End();
//}

void Application::updateViewport(Size size) {
    NestRen::Rect viewport = NestRen::Rect(
        0, 0, size.width, size.height
    );
    NestRen::setViewport(0, viewport);
}

class TriangleRenderer final {
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

    void initialize() {}

    void update(double deltaTime) {
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

void Application::loop() {
    TriangleRenderer level;
    while (!m_window->shouldClose()) {
        uint64_t lastTime = timeMillis;
        timeMillis = getMillis();
        deltaTimeMillis += timeMillis - lastTime;
        if (deltaTimeMillis < (1000ul / maximumFps)) {
            continue;
        }
        oneSecondTimeCount += deltaTimeMillis;

        thisSecondFramesCount++;
        if (oneSecondTimeCount >= 1000) {
            fps = thisSecondFramesCount;
            LOG_INFO("FPS: {}", fps);
            thisSecondFramesCount = 0;
            oneSecondTimeCount = 0;
        }

        double deltaTime = deltaTimeMillis / 1000.0;
        deltaTimeMillis = 0;

        if (Events::isJustKeyPressed(Key::ESCAPE)) {
            close();
        }
        if (Events::isJustKeyPressed(Key::TAB)) {
            Events::toggleCursorLock();
        }

        if (m_lastViewportSize != m_window->getSize()) {
            updateViewport(m_window->getSize());
        }

//        m_ImGuiLayer->begin(deltaTime);
        if (m_currentLevel) {
            deltaTime = std::min(deltaTime, 10.);
        }
        level.update(deltaTime);
//        drawProperties();
//        m_ImGuiLayer->end();

        NestRen::frame();
        NestRen::renderFrame();

        Events::resetDropPaths();
        Events::pollEvents();
        m_window->swapBuffers();
    }
}
void Application::close() {
    m_window->setShouldClose();
}

}