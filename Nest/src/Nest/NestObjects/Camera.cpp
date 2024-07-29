#include "Nest/NestObjects/Camera.hpp"

#include <glm/ext.hpp>
#include <iostream>

Camera::Camera()
    : fieldOfViewRadians(glm::radians(45.f))
    , aspect(1.f)
    , rotation(0.f)
    , position(0.f)
    , front(0.f)
    , right(0.f)
    , up(0.f) {
    updateVectors();
}

Camera::~Camera() {}

void Camera::setFieldOfView(float radians) {
    if (radians == this->fieldOfViewRadians) {
        return;
    }
    this->fieldOfViewRadians = radians;
}

void Camera::updateAspectRatio(float aspect) {
    if (aspect == this->aspect) {
        return;
    }
    this->aspect = aspect;
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(fieldOfViewRadians, aspect, 0.01f, 1000.f);
}

void Camera::rotate(float x, float y, float z) {
    if (x == 0 && y == 0 && z == 0) {
        return;
    }
    rotation += glm::vec3(x, y, z);
}

void Camera::setRotation(float x, float y, float z) {
    if (rotation.x == x && rotation.y == y && rotation.z == z) {
        return;
    }
    rotation.x = x;
    rotation.y = y;
    rotation.z = z;
}

void Camera::translate(float x, float y, float z) {
    if (x == 0 && y == 0 && z == 0) {
        return;
    }
    position += glm::vec3(x, y, z);
}

void Camera::translateLocal(float x, float y, float z) {
    if (x == 0 && y == 0 && z == 0) {
        return;
    }
    position += right * x;
    position += up * y;
    position += front * z;
}

void Camera::setPosition(float x, float y, float z) {
    if (position.x == x && position.y == y && position.z == z) {
        return;
    }
    position.x = x;
    position.y = y;
    position.z = z;
}

void Camera::updateVectors() {
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

glm::mat4 Camera::getViewMatrix() {
    glm::vec3 target = position + front;
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getSkyViewMatrix() {
    glm::mat4 rotationMatrix = glm::mat4(1.f);
    rotationMatrix =
        glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix =
        glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix =
        glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    return rotationMatrix;
}

glm::vec3 Camera::getPosition() {
    return position;
}

glm::vec3 Camera::getFront() {
    return front;
}
