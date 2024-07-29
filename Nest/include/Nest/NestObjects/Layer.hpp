#pragma once

class Layer {
public:
    virtual void start() = 0;
    virtual void update(double deltaTime) = 0;
    virtual void detach() = 0;
};
