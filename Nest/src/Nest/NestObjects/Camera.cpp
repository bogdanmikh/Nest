#include "Nest/NestObjects/Camera.hpp"

#include <glm/ext.hpp>
#include <iostream>

Camera::Camera()
    : shader(nullptr)
    , fieldOfViewRadians(glm::radians(45.f))
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
    updateProjectionMatrix();
}

void Camera::updateAspectRatio(float aspect) {
    if (aspect == this->aspect) {
        return;
    }
    this->aspect = aspect;
    updateProjectionMatrix();
}

void Camera::setShader(Shader *shader) {
    this->shader = shader;
    updateProjectionMatrix();
    updateViewMatrix();
}


Shader* Camera::getShader() {
    if (this->shader == nullptr) {
        printf("SHADER::NOT_INIT\n");
        return nullptr;
    }
    return this->shader;
}

void Camera::updateProjectionMatrix() {
    if (shader != nullptr) {
        glm::mat4 projection = glm::perspective(fieldOfViewRadians, aspect, 0.01f, 1000.f);
        shader->use();
        shader->setMat4("u_projection", projection);
    }
}

void Camera::rotate(float x, float y, float z) {
    if (x == 0 && y == 0 && z == 0) {
        return;
    }
    rotation += glm::vec3(x, y, z);
    updateVectors();
    updateViewMatrix();
}

void Camera::setRotation(float x, float y, float z) {
    if (rotation.x == x && rotation.y == y && rotation.z == z) {
        return;
    }
    rotation.x = x;
    rotation.y = y;
    rotation.z = z;
    updateVectors();
    updateViewMatrix();
}

void Camera::translate(float x, float y, float z) {
    if (x == 0 && y == 0 && z == 0) {
        return;
    }
    position += glm::vec3(x, y, z);
    updateViewMatrix();
}

void Camera::translateLocal(float x, float y, float z) {
    if (x == 0 && y == 0 && z == 0) {
        return;
    }
    position += right * x;
    position += up * y;
    position += front * z;
    updateViewMatrix();
}

void Camera::setPosition(float x, float y, float z) {
    if (position.x == x && position.y == y && position.z == z) {
        return;
    }
    position.x = x;
    position.y = y;
    position.z = z;
    updateViewMatrix();
}

void Camera::updateVectors() {
    glm::mat4 rotationMatrix = glm::mat4(1.f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    rotationMatrix = glm::transpose(rotationMatrix);
    front = rotationMatrix * glm::vec4(0.f, 0.f, -1.f, 1.f);
    right = rotationMatrix * glm::vec4(1.f, 0.f, 0.f, 1.f);
    up = rotationMatrix * glm::vec4(0.f, 1.f, 0.f, 1.f);
}

void Camera::updateViewMatrix() {
    if (shader != nullptr) {
        glm::vec3 target = position + front;
        glm::mat4 view = glm::lookAt(position, target, up);
        shader->use();
        shader->setMat4("u_view", view);
    }
}

glm::vec3 Camera::getPosition() {
    return position;
}

glm::vec3 Camera::getFront() {
    return front;
}
