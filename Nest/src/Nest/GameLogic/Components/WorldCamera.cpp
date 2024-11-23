//
// Created by Bogdan
//

#include "Nest/GameLogic/Components/WorldCamera.hpp"

namespace Nest {

WorldCamera::WorldCamera()
    : fieldOfViewRadians(glm::radians(45.f))
    , aspect(1.f)
    , rotation(0.f)
    , position(0.f)
    , front(0.f)
    , right(0.f)
    , up(0.f) {
    updateVectors();
}

WorldCamera::~WorldCamera() {}

void WorldCamera::setFieldOfView(float radians) {
    if (radians == this->fieldOfViewRadians) {
        return;
    }
    this->fieldOfViewRadians = radians;
}

void WorldCamera::updateAspectRatio(float aspect) {
    if (aspect == this->aspect) {
        return;
    }
    this->aspect = aspect;
}

glm::mat4 WorldCamera::getProjectionMatrix() {
    return glm::perspective(fieldOfViewRadians, aspect, 0.01f, 1000.f);
}

void WorldCamera::rotate(float x, float y, float z) {
    if (x == 0 && y == 0 && z == 0) {
        return;
    }
    rotation += glm::vec3(x, y, z);
}

void WorldCamera::setRotation(float x, float y, float z) {
    if (rotation.x == x && rotation.y == y && rotation.z == z) {
        return;
    }
    rotation.x = x;
    rotation.y = y;
    rotation.z = z;
}

void WorldCamera::translate(float x, float y, float z) {
    if (x == 0 && y == 0 && z == 0) {
        return;
    }
    position += glm::vec3(x, y, z);
}

void WorldCamera::translateLocal(float x, float y, float z) {
    if (x == 0 && y == 0 && z == 0) {
        return;
    }
    position += right * x;
    position += up * y;
    position += front * z;
}

void WorldCamera::setPosition(float x, float y, float z) {
    if (position.x == x && position.y == y && position.z == z) {
        return;
    }
    position.x = x;
    position.y = y;
    position.z = z;
}

void WorldCamera::updateVectors() {
    glm::mat4 rotationMatrix = glm::mat4(1.f);
    rotationMatrix =
        glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix =
        glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix =
        glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    rotationMatrix = glm::transpose(rotationMatrix);
    front = rotationMatrix * glm::vec4(0.f, 0.f, -1.f, 1.f);
    right = rotationMatrix * glm::vec4(1.f, 0.f, 0.f, 1.f);
    up = rotationMatrix * glm::vec4(0.f, 1.f, 0.f, 1.f);
}

glm::mat4 WorldCamera::getViewMatrix() {
    glm::vec3 target = position + front;
    return glm::lookAt(position, target, up);
}

glm::mat4 WorldCamera::getSkyViewMatrix() {
    glm::mat4 rotationMatrix = glm::mat4(1.f);
    rotationMatrix =
        glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix =
        glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix =
        glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    return rotationMatrix;
}

glm::vec3 WorldCamera::getPosition() {
    return position;
}

glm::vec3 WorldCamera::getFront() {
    return front;
}

} // namespace Nest
