#pragma once

#include "Bird/Bird.hpp"

#include <Foundation/Logger.hpp>

namespace Bird {

struct Uniform {
    ProgramHandle handle;
    const char *name;
    UniformType type;
    void *data;
    uint16_t count;
    uint16_t alignment;
    uint16_t size;

    Uniform(ProgramHandle handle, const char *name, void *data, UniformType type, uint16_t count)
        : handle(handle)
        , name(name)
        , data(data)
        , count(count)
        , type(type)
        , alignment(0)
        , size(0) {}
};

} // namespace Bird