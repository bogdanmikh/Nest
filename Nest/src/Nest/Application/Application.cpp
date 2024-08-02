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

    ImGui_Init(m_window->getNativeHandle());

    m_worldCamera = NEW(Foundation::getAllocator(), WorldCamera);
    m_worldCamera->setPosition(0, 0, 10);
    m_worldCamera->setFieldOfView(glm::radians(60.f));

    m_maximumFps = 60;
    m_oneSecondTimeCount = 0;
    m_deltaTimeMillis = 0;
    m_thisSecondFramesCount = 0;
    m_timeMillis = getMillis();
    m_lastViewportSize = m_window->getSize();
}

Application::~Application() {
    ImGui_Shutdown();
    if (m_layer) m_layer->onDetach();
    FREE(Foundation::getAllocator(), m_worldCamera);
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
        uint64_t lastTime = m_timeMillis;
        m_timeMillis = getMillis();
        m_deltaTimeMillis += m_timeMillis - lastTime;
        if (m_deltaTimeMillis < (1000 / m_maximumFps)) {
            continue;
        }
        m_oneSecondTimeCount += m_deltaTimeMillis;

        m_thisSecondFramesCount++;
        if (m_oneSecondTimeCount >= 1000) {
            fps = m_thisSecondFramesCount;
            LOG_INFO("FPS: {}", fps);
            m_thisSecondFramesCount = 0;
            m_oneSecondTimeCount = 0;
        }
        double deltaTime = m_deltaTimeMillis / 1000.0;
        if (deltaTime == 0) {
            deltaTime = 0.00000001;
        }
        m_deltaTimeMillis = 0;

        if (Events::isJustKeyPressed(Key::ESCAPE)) {
            close();
        }
        if (Events::isJustKeyPressed(Key::TAB)) {
            Events::toggleCursorLock();
        }

        if (m_lastViewportSize != m_window->getSize()) {
            updateViewport(m_window->getSize());
        }

        m_worldCamera->update();
        if (deltaTime >= 2) {
            LOG_ERROR("Delta time: {}", deltaTime);
        }
        ImGui_NewFrame();
        if (m_layer) {
            m_layer->onUpdate(deltaTime);
        }
        Events::resetDropPaths();
        NestRen::renderFrame();
        ImGui_EndFrame();
        NestRen::frame();
        NestRen::flip();
        Events::pollEvents();
    }
}

void Application::close() {
    m_window->setShouldClose();
}

}