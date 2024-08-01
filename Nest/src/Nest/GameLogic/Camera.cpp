#include "Nest/GameLogic/Camera.hpp"

namespace Nest {

static const float zNearDefault = 0.1f;
static const float zFarDefault = 1000.f;

Camera::Camera(Size viewportSize)
    : m_viewportSize(viewportSize)
    , m_projection(1.f)
    , m_zNear(zNearDefault)
    , m_zFar(zFarDefault) {}

float Camera::getNear() {
    return m_zNear;
}

float Camera::getFar() {
    return m_zFar;
}

void Camera::setFar(float far) {
    m_zFar = far;
    updateProjectionMatrix();
}

void Camera::setNear(float near) {
    m_zNear = near;
    updateProjectionMatrix();
}

glm::mat4 Camera::getProjection() {
    return m_projection;
}

Size Camera::getViewportSize() {
    return m_viewportSize;
}

void Camera::setViewportSize(Size size) {
    m_viewportSize = size;
    updateProjectionMatrix();
}

Vec3 Camera::screenCoordToWorld(glm::mat4 view, Vec2 screen, float distance) {
    glm::vec4 in;
    in.x = 2.f * screen.x / m_viewportSize.width - 1.f;
    in.y = 1.f - 2.f * screen.y / m_viewportSize.height;
    // То же самое ниже
    // glm::vec4 temp = m_projection * glm::vec4(0.f, 0.f, -distance, 1.f);
    // in.z = temp.z / temp.w;
    float zValue = -distance * -(m_zFar + m_zNear) / (m_zFar - m_zNear) -
                   (2 * m_zFar * m_zNear) / (m_zFar - m_zNear);
    float wValue = distance;
    in.z = zValue / wValue;
    in.w = 1.f;
    glm::mat4 viewProjection = m_projection * view;
    glm::mat4 viewProjInverse = glm::inverse(viewProjection);
    glm::vec4 result = viewProjInverse * in;
    result /= result.w;
    return Vec3(result.x, result.y, result.z);
}

Vec3 Camera::screenCoordToWorld(glm::mat4 view, Vec3 screen) {
    glm::vec4 in;
    in.x = 2.f * screen.x / m_viewportSize.width - 1.f;
    in.y = 1.f - 2.f * screen.y / m_viewportSize.height;
    in.z = screen.z;
    in.w = 1.f;
    glm::mat4 viewProjection = m_projection * view;
    glm::mat4 viewProjInverse = glm::inverse(viewProjection);
    glm::vec4 result = viewProjInverse * in;
    result /= result.w;
    return Vec3(result.x, result.y, result.z);
}

} // namespace Nest