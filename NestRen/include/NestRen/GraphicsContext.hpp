//
// Created by Admin on 18.02.2022.
//

#pragma once

#include <cstdint>

namespace NestRen {
class GraphicsContext {
public:
    virtual ~GraphicsContext() = default;
    virtual void create() = 0;
    virtual uint32_t getDefaultFrameBufferId() = 0;
};
} // namespace Miren
