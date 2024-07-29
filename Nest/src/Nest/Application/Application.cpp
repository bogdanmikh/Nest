#include <chrono>

#include "Nest/Application/Application.hpp"
#include "Nest/Logger/Logger.hpp"
#include "Nest/Renderer/Renderer.hpp"
#include "Nest/Window/Events.hpp"
#include "Nest/ImGui/ImGui.hpp"
#include "imgui.h"

Application *Application::s_instance = new Application;
std::shared_ptr<spdlog::logger> Logger::s_logger = nullptr;

uint64_t getMillis() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );
    return now.count();
}

Application::Application()
    : currentLayer(nullptr)
    , fps()
    , timeMillis() {
    Logger::init();
    window = new Window;
    window->init("Light", 800, 600, false);

    camera = new Camera;
    camera->setFieldOfView(glm::radians(60.f));
    camera->setRotation(0.f, 50.f, 0.f);

    ImGui_Init(window->getNativeHandle());
    Renderer::init();
    Renderer::setClearColor(.235f, .235f, .235f, 1.0f);
    timeMillis = getMillis();
    m_lastViewportSize = window->getSize();
}

Application::~Application() {
    ImGui_Shutdown();
    delete camera;
    delete window;
}

void Application::drawProperties() const {
    ImGui::Begin("Stats");
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    ImGui::Text("FPS: %d", fps);
    ImGui::PopStyleColor();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.9f, 0.0f, 1.0f));
    ImGui::Text(
        "X: %f, Y: %f, Z: %f",
        camera->getPosition().x,
        camera->getPosition().y,
        camera->getPosition().z
    );
    ImGui::PopStyleColor();
    ImGui::End();
}

void Application::updateViewport() {
    glm::vec2 currSize = Application::getInstance()->getWindow()->getSize();
    if (m_lastViewportSize != currSize) {
        m_lastViewportSize = currSize;
        Renderer::setRenderBufferSize(currSize.x, currSize.y);
    }
}

void Application::loop() {
    while (!window->shouldClose()) {
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

        Renderer::clear();

        updateViewport();

        ImGui_NewFrame();
        camera->update();
        if (currentLayer) {
            deltaTime = std::min(deltaTime, 10.);
            currentLayer->update(deltaTime);
        }
        drawProperties();
        ImGui_EndFrame();

        Events::resetDropPaths();
        Events::pollEvents();
        window->swapBuffers();
    }
}
void Application::close() {
    window->setShouldClose();
}
