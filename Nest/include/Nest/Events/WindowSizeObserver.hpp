//
// Created by Admin on 08.02.2022.
//

#pragma once

#include "Nest/Base/Base.hpp"

namespace Nest {

class WindowSizeObserver {
public:
    virtual void windowSizeChanged(Size size) = 0;
};

} // namespace Nest
