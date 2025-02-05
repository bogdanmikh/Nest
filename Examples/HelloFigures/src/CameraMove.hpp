#pragma once

#include <Nest.hpp>

class CameraMove : public Nest::Entity {
public:
    void onAttach() override;
    void onDetach() override {}
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;

private:
    double dt = 1.;
    bool cursorLock;
    glm::vec2 lastPos;
    float cameraSpeed = 5.f;

    Nest::Window *m_window;
    Nest::WorldCamera *m_worldCamera;
};
