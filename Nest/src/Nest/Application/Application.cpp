#include <chrono>

#include "Nest/ImGui/ImGui.hpp"
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

    m_layer = nullptr;
    Foundation::Logger::init();
    m_window = NEW(Foundation::getAllocator(), Window);
    m_window->init(settings.name, settings.windowSize.x, settings.windowSize.y, settings.isFullScreen);
    NestRen::initialize();
    Events::init(m_window->getNativeHandle());
    ImGui_Init(m_window->getNativeHandle());

    timeMillis = getMillis();
    m_lastViewportSize = m_window->getSize();
}

Application::~Application() {
//    m_ImGuiLayer->onDetach();
    ImGui_Shutdown();
    if (m_layer) m_layer->onDetach();
    FREE(Foundation::getAllocator(), m_layer);
    FREE(Foundation::getAllocator(), m_window);
}

void Application::updateViewport(Size size) {
    NestRen::Rect viewport = NestRen::Rect(
        0, 0, size.width, size.height
    );
    NestRen::setViewport(0, viewport);
}

void Application::loop() {
    if (m_layer) m_layer->onAttach();
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

        ImGui_NewFrame();
        if (m_layer) {
            m_layer->onUpdate(deltaTime);
        }
        ImGui::SetNextWindowPos({100, 100});
        ImGui::SetNextWindowSize({100, 100});
        ImGui_EndFrame();

//        Events::resetDropPaths();
        Events::pollEvents();

        NestRen::renderFrame();
        NestRen::frame();
    }
}

void Application::close() {
    m_window->setShouldClose();
}

}