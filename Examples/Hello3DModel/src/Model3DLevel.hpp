#pragma once

#include <Nest.hpp>

class CameraMove;
class SnowScene;

class Model3DLevel final : public Nest::Layer {
public:
    ~Model3DLevel() override {}
    void onAttach() override;
    void onUpdate(double deltaTime) override;
    void onDetach() override;
    void addEntity(Nest::Entity *entity) override;

private:
    std::vector<Nest::Entity *> m_entities;
    CameraMove *m_cameraMove;
    SnowScene *m_carRenderer;
};