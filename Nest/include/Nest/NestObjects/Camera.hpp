#pragma once

#include <glm/glm.hpp>
#include "Nest/NestRenderer/Shader.hpp"

class Camera {
public:
    Camera();
    ~Camera();

    void setFieldOfView(float radians);
    void updateAspectRatio(float aspect);
    void setShader(Shader* shader);
    Shader* getShader();
    void rotate(float x, float y, float z);
    void setRotation(float x, float y, float z);
    void translate(float x, float y, float z);
    void setPosition(float x, float y, float z);
    void translateLocal(float x, float y, float z);

    glm::vec3 getPosition();
    glm::vec3 getFront();

private:
    void updateVectors();
    void updateViewMatrix();
    void updateProjectionMatrix();

    glm::vec3 rotation;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    Shader* shader;
    float fieldOfViewRadians;
    float aspect;
};