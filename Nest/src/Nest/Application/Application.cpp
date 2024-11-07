#include "Nest/Application/Application.hpp"

#include "Nest/ImGui/ImGui.hpp"
#include "Nest/Platform/PlatformDetection.hpp"
#ifdef PLATFORM_DESKTOP
#    include "Platform/WindowImpl/GlfwWindow/GlfwWindow.hpp"
#    include "Platform/EventsImpl/GlfwEvents/GlfwEvents.hpp"
#elif defined(PLATFORM_ANDROID)
 #    include "Platform/WindowImpl/AndroidWindow/AndroidWindow.hpp"
 #    include "Platform/EventsImpl/AndroidEvents/AndroidEvents.hpp"
#endif

#include <Bird/Bird.hpp>
#include <Foundation/Logger.hpp>
#include <Foundation/Allocator.hpp>

#include <chrono>

namespace Nest {

Application *Application::s_instance = nullptr;

uint64_t getMillis() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );
    return now.count();
}

Application::Application(ApplicationStartupSettings &settings) {
    std::srand(static_cast<unsigned int>(std::time(0)));

    s_instance = this;

    m_layer = nullptr;
    Foundation::Logger::init();

#ifdef PLATFORM_DESKTOP
    m_window = NEW(Foundation::getAllocator(), GlfwWindow);
    m_window->init(
        settings.name, settings.windowSize.x, settings.windowSize.y, settings.isFullScreen
    );

    m_events = NEW(Foundation::getAllocator(), GlfwEvents);
    m_events->init(m_window->getNativeHandle());
#elif defined(PLATFORM_ANDROID)
    m_window = NEW(Foundation::getAllocator(), AndroidWindow);
    m_window->init(
        settings.name, settings.windowSize.x, settings.windowSize.y, settings.isFullScreen
    );
    m_events = NEW(Foundation::getAllocator(), AndroidEvents);
    m_events->init(m_window->getNativeHandle());
#endif

    Bird::initialize();

    ImGui_Init(m_window->getNativeHandle());

    m_worldCamera = NEW(Foundation::getAllocator(), WorldCamera);
    m_worldCamera->setPosition(0, 0, 0);
    m_worldCamera->setFieldOfView(glm::radians(60.f));
    m_worldCamera->updateAspectRatio(m_window->getSize().x / m_window->getSize().y);

    m_maximumFps = 60;
    m_oneSecondTimeCount = 0;
    m_deltaTimeMillis = 0;
    m_thisSecondFramesCount = 0;
    m_timeMillis = getMillis();
    m_lastViewportSize = m_window->getSize();
}

Application::~Application() {
    ImGui_Shutdown();
    if (m_layer) {
        m_layer->onDetach();
    }
    DELETE(Foundation::getAllocator(), m_worldCamera);
    DELETE(Foundation::getAllocator(), m_window);
    delete m_layer;
}

void Application::updateViewport(Size size) {
    Bird::Rect viewport = Bird::Rect(0, 0, size.width, size.height);
    Bird::setViewport(0, viewport);
}

void Application::loop() {
    if (m_layer != nullptr) {
        m_layer->onAttach();
    }
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
        if (deltaTime >= 2) {
            LOG_ERROR("Delta time: {}", deltaTime);
        }
        m_deltaTimeMillis = 0;

        if (m_events->isJustKeyPressed(Key::ESCAPE)) {
            close();
        }
        if (m_events->isJustKeyPressed(Key::TAB)) {
            m_events->toggleCursorLock();
        }

        if (m_lastViewportSize != m_window->getSize()) {
            updateViewport(m_window->getSize());
        }

        m_worldCamera->update();
        ImGui_NewFrame();
        if (m_layer) {
            m_layer->onUpdate(deltaTime);
        }
        m_events->resetDropPaths();
        Bird::renderFrame();
        ImGui_EndFrame();
        Bird::frame();
        Bird::flip();
        m_events->pollEvents();
    }
}

void Application::close() {
    m_window->setShouldClose();
}

} // namespace Nest