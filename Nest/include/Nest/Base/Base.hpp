#pragma once

#include <Rain/Codable.hpp>
#include <Rain/Rain.hpp>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>
#include <random>

namespace Nest {

using Path = std::filesystem::path;

struct Vec2 : public Rain::Codable {
    union {
        float x, width;
    };
    union {
        float y, height;
    };

    Vec2()
        : x(0)
        , y(0) {}

    Vec2(glm::vec2 c)
        : x(c.x)
        , y(c.y) {}

    Vec2(float x, float y)
        : x(x)
        , y(y) {}

    Vec2(const ImVec2 &vec)
        : x(vec.x)
        , y(vec.y) {}

    Vec2 operator+(const Vec2 &l) {
        return {x + l.x, y + l.y};
    }

    Vec2 operator-(const Vec2 &l) {
        return {x - l.x, y - l.y};
    }

    void operator+=(const Vec2 &l) {
        x += l.x;
        y += l.y;
    }

    void operator-=(const Vec2 &l) {
        x -= l.x;
        y -= l.y;
    }

    bool operator==(const Vec2 &l) {
        return l.x == x && l.y == y;
    }

    bool operator!=(const Vec2 &l) {
        return !(*this == l);
    }

    void operator*=(const Vec2 &l) {
        x *= l.x;
        y *= l.y;
    }

    Vec2 operator*(const Vec2 &l) {
        return {x * l.x, y * l.y};
    }

    operator ImVec2() {
        return {x, y};
    }

    operator glm::vec2() {
        return {x, y};
    }

    RAIN_FIELDS_BEGIN(Vec2)
    RAIN_FIELD(x)
    RAIN_FIELD(y)
    RAIN_FIELDS_END
};

struct Vec3 : public Rain::Codable {
    union {
        float x, r, width;
    };
    union {
        float y, g, height;
    };
    union {
        float z, b;
    };

    Vec3()
        : x(0)
        , y(0)
        , z(0) {}

    Vec3(glm::vec3 c)
        : x(c.x)
        , y(c.y)
        , z(c.z) {}

    Vec3(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z) {}

    operator glm::vec3() {
        return {x, y, z};
    }

    RAIN_FIELDS_BEGIN(Vec3)
    RAIN_FIELD(x)
    RAIN_FIELD(y)
    RAIN_FIELD(z)
    RAIN_FIELDS_END
};

struct Vec4 : public Rain::Codable {
    union {
        float x, r, width;
    };
    union {
        float y, g, height;
    };
    union {
        float z, b;
    };
    union {
        float w, a;
    };

    Vec4()
        : x(0)
        , y(0)
        , z(0)
        , w(0) {}

    Vec4(glm::vec4 c)
        : x(c.x)
        , y(c.y)
        , z(c.z)
        , w(c.w) {}

    Vec4(glm::quat c)
        : x(c.x)
        , y(c.y)
        , z(c.z)
        , w(c.w) {}

    Vec4(float x, float y, float z, float w)
        : x(x)
        , y(y)
        , z(z)
        , w(w) {}

    operator glm::vec4() {
        return {x, y, z, w};
    }

    operator glm::quat() {
        glm::quat q;
        q.x = x;
        q.y = y;
        q.z = z;
        q.w = w;
        return q;
    }

    RAIN_FIELDS_BEGIN(Vec4)
    RAIN_FIELD(x)
    RAIN_FIELD(y)
    RAIN_FIELD(z)
    RAIN_FIELD(w)
    RAIN_FIELDS_END
};

using id_t = int32_t;
using Size = Vec2;
using Color = Vec4;
using Quat = Vec4;

static int getRandomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

static float getRandomFloat(float min, float max) {
    float random = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

    return min + random * (max - min);
}

} // namespace Nest