#pragma once

class GameObject {
public:
    explicit GameObject() = default;
    virtual ~GameObject() = default;
    virtual void update(double deltaTime) = 0;
    virtual void draw() = 0;
};