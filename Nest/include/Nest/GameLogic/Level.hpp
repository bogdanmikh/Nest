//
// Created by Admin on 25.01.2022.
//

#pragma once

#include "Nest/GameLogic/World.hpp"

namespace Nest {

class Level {
public:
    virtual ~Level() = default;
    virtual void start(World *world) = 0;
};

} // namespace Panda