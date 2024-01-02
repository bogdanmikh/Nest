#pragma once

#include "../../Nest/include/Nest.hpp"

class Level {
public:
    virtual ~Level() = default;
    virtual void start (Window *window) = 0;
};