#pragma once

class Level {
public:
    virtual void start() = 0;
    virtual void update(double deltaTime) = 0;
};
