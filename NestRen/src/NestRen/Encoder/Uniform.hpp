#pragma once

#include "NestRen/NestRen.hpp"

#include <Foundation/Logger.hpp>

namespace NestRen {

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

    Uniform()
        : handle(0)
        , name(nullptr)
        , data(nullptr)
        , type(UniformType::Sampler)
        , size(0) {}
};

} // namespace NestRen