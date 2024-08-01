//
// Created by Admin on 09.02.2022.
//

#include "Panda/GameLogic/Components/TransformComponent.hpp"

namespace Panda {

TransformComponent::TransformComponent()
    : rotation(1.0f, 0.0f, 0.0f, 0.0f)
    , rotationEuler(0.0f, 0.0f, 0.0f)
    , position(0.0f, 0.0f, 0.0f)
    , scale(1.0f, 1.0f, 1.0f) {}

void TransformComponent::setRotationEuler(glm::vec3 degrees) {
    rotationEuler = glm::radians(degrees);
    rotation = glm::quat(rotationEuler);
}

void TransformComponent::rotateEuler(glm::vec3 degrees) {
    rotationEuler += glm::radians(degrees);
    rotation = glm::quat(rotationEuler);
}

glm::vec3 TransformComponent::getRotationEuler() {
    return glm::degrees(rotationEuler);
}

void TransformComponent::setRotation(glm::quat quat) {
    rotation = quat;
    rotationEuler = glm::eulerAngles(rotation);
}

glm::quat TransformComponent::getRotation() {
    return rotation;
}

glm::mat4 TransformComponent::getRotationMatrix() {
    return glm::toMat4(rotation);
}

void TransformComponent::translate(glm::vec3 offset) {
    position += offset;
}

void TransformComponent::setPosition(glm::vec3 pos) {
    position = pos;
}

glm::vec3 TransformComponent::getPosition() {
    return position;
}

glm::vec4 TransformComponent::getPositionHomogeneous() {
    return {position.x, position.y, position.z, 1.0f};
}

glm::mat4 TransformComponent::getTransform() const {
    return glm::translate(glm::mat4(1.0f), position) * glm::toMat4(rotation) *
           glm::scale(glm::mat4(1.0f), scale);
}

glm::mat4 TransformComponent::getSkyTransform() const {
    return glm::toMat4(rotation) * glm::scale(glm::mat4(1.0f), scale);
}

glm::vec3 TransformComponent::getScale() {
    return scale;
}

void TransformComponent::setScale(glm::vec3 _scale) {
    scale = _scale;
}

} // namespace Panda