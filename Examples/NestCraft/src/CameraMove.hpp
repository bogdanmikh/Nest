#pragma once

#include <Nest.hpp>

class CameraMove : public Nest::Entity {
public:
    void onAttach() override;
    void onDetach() override {}
    void onUpdate(double deltaTime) override;
    void onImGuiRender() override;

private:
    bool cursorLock;
    Nest::Vec2 lastPos;
    float cameraSpeed = 5.f;
    double dt;

    Nest::Window *m_window;
    Nest::WorldCamera *m_worldCamera;
};
