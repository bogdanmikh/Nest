#pragma once

#include "Nest/GameLogic/Camera.hpp"
#include "Nest/Base/Base.hpp"

namespace Nest {

class WorldCamera : public Camera {
public:
    enum class ProjectionType { PERSPECTIVE = 0, ORTHOGRAPHIC = 1 };
    WorldCamera();
    ProjectionType getProjectionType() const;
    void setProjectionType(ProjectionType type);
    // MARK: Perspective parameters
    void setFieldOfView(float degrees);
    float getFieldOfView();
    // MARK: Orthographic parameters
    float getOrthoSize();
    void setOrthoSize(float orthoSize);

private:
    void updateProjectionMatrix() override;

    ProjectionType m_projectionType;
    float m_fieldOfView;
    float m_orthoSize;
};

} // namespace Panda