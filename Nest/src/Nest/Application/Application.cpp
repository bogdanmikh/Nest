#include <chrono>

#include "Nest/Application/Application.hpp"
#include <NestUI.hpp>

uint64_t getMillis() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    return now.count();
}

Application::Application() {
    window = new Window;
    window->init("Sandbox", 640, 480, true);

    camera = new Camera;
    camera->setFieldOfView(glm::radians(45.f));
    camera->setRotation(0.f, 0.f, 0.f);
//    camera
    ImGui_Init(window->getNativeHandle());

    Renderer::init();

    Renderer::setClearColor(.235f, .235f, .235f, 1.0f);
}

Application::~Application() {
    ImGui_Shutdown();
    delete camera;
    delete window;
    delete s_instance;
}

void Application::loop() {
    while (!window->shouldClose()) {
        uint64_t lastTime = timeMillis;
        timeMillis = getMillis();
        deltaTimeMillis += timeMillis - lastTime;
        if (deltaTimeMillis < (1000 / maximumFps)) {
            continue;
        }
        oneSecondTimeCount += deltaTimeMillis;

        thisSecondFramesCount++;
        if (oneSecondTimeCount >= 1000) {
            fps = thisSecondFramesCount;
            std::cout << "FPS: " << fps << std::endl;
            thisSecondFramesCount = 0;
            oneSecondTimeCount = 0;
        }

        double deltaTime = deltaTimeMillis / 1000.0;
        deltaTimeMillis = 0;

        Renderer::clear();

        if (window->isKeyPressed(Key::ESCAPE)) {
            close();
        }
        ImGui_NewFrame();
        if (currentLevel != nullptr) {
            currentLevel->update(deltaTime);
        }
        ImGui_EndFrame();


        window->swapBuffers();
        window->pollEvents();
    }
}

void Application::close() {
    window->setShouldClose();
}

