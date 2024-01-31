#pragma once

#include <Nest.hpp>

class Menu : public GameObject {
public:
    Menu();
    void update(double deltaTime) override;
    void draw() override {}
private:
    float m_color[3];
};
