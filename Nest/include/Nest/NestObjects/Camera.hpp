#pragma once

#include <glm/glm.hpp>

class Camera {
public:
    Camera();
    ~Camera();

    void update() {
        updateVectors();
    }

    void setFieldOfView(float radians);
    void updateAspectRatio(float aspect);
    void rotate(float x, float y, float z);
    void setRotation(float x, float y, float z);
    void translate(float x, float y, float z);
    void setPosition(float x, float y, float z);
    void translateLocal(float x, float y, float z);
    glm::mat4 getViewMatrix();
    glm::mat4 getSkyViewMatrix();
    glm::mat4 getProjectionMatrix();

    glm::vec3 getPosition();
    glm::vec3 getFront();

private:
    void updateVectors();

    glm::vec3 rotation;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    float fieldOfViewRadians;
    float aspect;
};