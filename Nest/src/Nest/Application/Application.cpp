#include "Nest/Application/Application.hpp"

#include "Nest/Platform/PlatformDetection.hpp"
#include "Nest/Application/Initialization/PlatformInit.hpp"
#include "Nest/GameLogic/Input.hpp"
#include "Nest/Events/WindowEvents.hpp"
#include <Foundation/PlatformDetection.hpp>
#include "Bird/PlatformData.hpp"

#include <Bird/Bird.hpp>
#include <Foundation/Allocator.hpp>
#include <Foundation/Logger.hpp>

#include <chrono>

namespace Nest {

Application *Application::s_instance = nullptr;

uint64_t getMillis() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );
    return now.count();
}

Application::Application(ApplicationStartupSettings &settings)
    : m_isApplicationShouldClose(false) {
    std::srand(static_cast<unsigned int>(std::time(0)));

    s_instance = this;

    m_layer = nullptr;

    Foundation::Logger::init();

    m_window = createWindow(settings);
    m_window->setEventQueue(&m_eventQueue);

    Bird::initialize();

    m_ImGuiLayer = F_NEW(Foundation::getAllocator(), ImGuiLayer);
    m_ImGuiLayer->onAttach();

    m_worldCamera = F_NEW(Foundation::getAllocator(), WorldCamera);
    m_worldCamera->setPosition(0, 0, 0);
    m_worldCamera->setFieldOfView(glm::radians(60.f));
    m_worldCamera->updateAspectRatio(m_window->getSize().x / m_window->getSize().y);

    m_maximumFps = 60;
    m_oneSecondTimeCount = 0;
    m_deltaTimeMillis = 0;
    m_thisSecondFramesCount = 0;
    m_timeMillis = getMillis();
}

Application::~Application() {
    m_ImGuiLayer->onDetach();
    if (m_layer) {
        m_layer->onDetach();
    }
    F_DELETE(Foundation::getAllocator(), m_ImGuiLayer);
    F_DELETE(Foundation::getAllocator(), m_worldCamera);
    F_DELETE(Foundation::getAllocator(), m_window);
    delete m_layer;
}

void Application::windowSizeChanged(Size size) {
    Bird::Rect viewport =
        Bird::Rect(0, 0, size.width * m_window->getDpi().x, size.height * m_window->getDpi().y);
    Bird::setViewport(0, viewport);
}

void Application::loop() {
    if (m_layer != nullptr) {
        m_layer->onAttach();
    }
    while (!m_isApplicationShouldClose) {
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

        if (Input::isKeyJustPressed(Key::ESCAPE)) {
            close();
        }
        if (Input::isKeyJustPressed(Key::TAB)) {
            m_window->toggleCursorLock();
        }

        if (m_window->shouldClose()) {
            close();
        }

        m_worldCamera->update();
        m_ImGuiLayer->onUpdate(deltaTime);
        m_ImGuiLayer->begin(deltaTime);
        if (m_layer) {
            m_layer->onImGuiRender();
            m_layer->onUpdate(deltaTime);
        }
        m_ImGuiLayer->end();
        m_window->pollEvents();
        Input::nextFrame();
        processEvents();
        Bird::frame();
    }
}

void Application::processEvents() {
    m_eventQueue.finishWriting();
    Event *event;
    while ((event = m_eventQueue.poll()) != nullptr) {
        if (event->type == EventType::WindowResize) {
            const WindowResizeEvent *ev = static_cast<const WindowResizeEvent *>(event);
            windowSizeChanged(Size(ev->getWidth(), ev->getHeight()));
        }
        if (!event->isHandled) {
            m_ImGuiLayer->onEvent(event);
            m_layer->onEvent(event);
        }
        //        if (!event->isHandled) {
        Input::onEvent(event);
        //        }
    }
    m_eventQueue.reset();
}

EventQueue *Application::getEventQueue() {
    return &m_eventQueue;
}

} // namespace Nest