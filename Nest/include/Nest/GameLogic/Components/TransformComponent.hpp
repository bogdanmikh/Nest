//
// Created by Bogdan
//

#pragma once

#include <glm/gtx/quaternion.hpp>

namespace Nest {

struct TransformComponent {
public:
    TransformComponent();
    // ------- Rotation -------
    void setRotationEuler(glm::vec3 degrees);
    glm::vec3 getRotationEuler();
    void rotateEuler(glm::vec3 degrees);
    void setRotation(glm::quat quat);
    glm::quat getRotation();
    glm::mat4 getRotationMatrix();
    // ------- Position -------
    glm::vec3 getPosition();
    glm::vec4 getPositionHomogeneous();
    void translate(glm::vec3 offset);
    void setPosition(glm::vec3 pos);
    // ------- Scale -------
    glm::vec3 getScale();
    void setScale(glm::vec3 scale);
    // ------- Matrices -------
    glm::mat4 getTransform() const;
    glm::mat4 getSkyTransform() const;

private:
    glm::quat rotation;
    // rotation in degrees
    glm::vec3 rotationEuler;
    glm::vec3 position;
    glm::vec3 scale;
};

} // namespace Nest