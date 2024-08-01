//
// Created by Admin on 09.02.2022.
//

#pragma once

#include <Foundation/Foundation.hpp>

namespace Nest {

class NativeScript {
public:
    NativeScript() {}

    virtual ~NativeScript() = default;

    virtual void initialize() = 0;

    virtual void update(double deltaTime) = 0;

    virtual void onImGuiRender() {};
};

} // namespace Panda