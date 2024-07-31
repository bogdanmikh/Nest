#pragma once

#include <imgui.h>
#include <glm/glm.hpp>

struct Vec2 {
    union {
        float x;
        float width;
    };
    union {
        float y;
        float height;
    };

    Vec2()
        : x(0)
        , y(0) {}
    Vec2(float x, float y)
        : x(x)
        , y(y) {}
    Vec2(ImVec2 &vec)
        : x(vec.x)
        , y(vec.y) {}

    operator ImVec2() const {
        return {x, y};
    }

    operator glm::vec2() const {
        return {x, y};
    }

    void operator=(ImVec2 vec) {
        x = vec.x;
        y = vec.y;
    }

    bool operator==(const Vec2 &vec) const {
        return x == vec.x && y == vec.y;
    }
};

