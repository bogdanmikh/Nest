#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Nest.hpp"

class Cube : public GameObject {
private:
    Shader *mShader;
    Mesh *mesh;
    glm::vec3 mRotation;
    glm::vec3 mPosition;

    glm::vec3 mSize;
    void updateModelMatrix();
public:
    Cube();
    ~Cube() {
        delete mesh;
    }

    void init();

    inline void addTexture(const std::string& texturePath) {
        mesh->addTexture(texturePath);
    }

    inline void setShader(Shader* shader) {
        mShader = shader;
    }

    void draw() override;

    inline void setPosition(float x, float y, float z) {
        mPosition = {x, y, z};
    }

    inline void setPosition(glm::vec3 position) {
        mPosition = position;
    }
    inline glm::vec3 getPosition() const {
        return mPosition;
    }

    inline void translate(glm::vec3 distance) {
        mPosition += distance;
    }
    inline void translate(float x, float y, float z) {
        mPosition += glm::vec3(x, y, z);
    }

    inline void setSize(float width, float height, float depth) {
        mSize = {width, height, depth};
    }

    inline void setRotation(glm::vec3 degree) {
        mRotation = degree;
    }

    inline glm::vec3 getRotation() const {
        return mRotation;
    }

    void update(double deltaTime) override {}
};
