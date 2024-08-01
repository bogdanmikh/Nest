//
// Created by Michael Andreichev on 30.11.2023.
//

#pragma once

#include <glm/glm.hpp>

#include "Nest/Base/Base.hpp"

namespace Nest {

class Camera {
public:
    Camera(Size viewportSize);
    virtual ~Camera() = default;
    virtual void updateProjectionMatrix() = 0;

    glm::mat4 getProjection();
    Size getViewportSize();
    void setViewportSize(Size size);
    float getNear();
    void setNear(float near);
    float getFar();
    void setFar(float far);

    // MARK: Math
    /// Первый вариант: дистанция от камеры задается в мировых координатах
    Vec3 screenCoordToWorld(glm::mat4 view, Vec2 screen, float distance);
    /// Второй вариант: дистанция от камеры задается буфером глубины.
    /// Значение z от -1 до 1 для OpenGL или от 0 до 1 для Vulkan.
    Vec3 screenCoordToWorld(glm::mat4 view, Vec3 screen);

protected:
    glm::mat4 m_projection;
    Size m_viewportSize;
    float m_zNear;
    float m_zFar;
};

} // namespace Panda
