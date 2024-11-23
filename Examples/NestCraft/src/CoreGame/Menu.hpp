#pragma once

#include <Nest.hpp>

class Menu {
public:
    Menu();
    void update();
    glm::vec3 getColor();

private:
    float m_color[3];
};
