//
// Created by Admin on 11.02.2022.
//

#include "Nest/GameLogic/Components/WorldCamera.hpp"

namespace Nest {

WorldCamera::WorldCamera()
    : Camera({200, 300})
    , m_fieldOfView(70.f)
    , m_projectionType(ProjectionType::PERSPECTIVE)
    , m_orthoSize(1.f) {}

void WorldCamera::setFieldOfView(float degrees) {
    m_fieldOfView = degrees;
    if (m_projectionType == ProjectionType::PERSPECTIVE) {
        updateProjectionMatrix();
    }
}

void WorldCamera::updateProjectionMatrix() {
    float aspectRatio = m_viewportSize.width / m_viewportSize.height;
    switch (m_projectionType) {
        case ProjectionType::PERSPECTIVE: {
            m_projection =
                glm::perspective<float>(glm::radians(m_fieldOfView), aspectRatio, m_zNear, m_zFar);
            break;
        }
        case ProjectionType::ORTHOGRAPHIC:
            float orthoLeft = -m_orthoSize * aspectRatio * 0.5f;
            float orthoRight = m_orthoSize * aspectRatio * 0.5f;
            float orthoBottom = -m_orthoSize * 0.5f;
            float orthoTop = m_orthoSize * 0.5f;

            m_projection =
                glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_zNear, m_zFar);
            break;
    }
}

WorldCamera::ProjectionType WorldCamera::getProjectionType() const {
    return m_projectionType;
}

void WorldCamera::setProjectionType(WorldCamera::ProjectionType type) {
    m_projectionType = type;
    updateProjectionMatrix();
}

void WorldCamera::setOrthoSize(float orthoSize) {
    m_orthoSize = orthoSize;
    if (m_projectionType == ProjectionType::ORTHOGRAPHIC) {
        updateProjectionMatrix();
    }
}

float WorldCamera::getFieldOfView() {
    return m_fieldOfView;
}

float WorldCamera::getOrthoSize() {
    return m_orthoSize;
}

} // namespace Panda
