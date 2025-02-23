#pragma once

#include "Bird/Bird.hpp"

#include <Foundation/Logger.hpp>

namespace Bird {

const static uint32_t s_uniformTypeSize[UniformType::Count + 1] = {
    sizeof(int32_t),       // Sampler
    4 * sizeof(float),     // Vec4
    3 * 3 * sizeof(float), // Mat3
    4 * 4 * sizeof(float), // Mat4
};

struct Uniform {
    ProgramHandle handle;
    std::string name;
    UniformType type;
    void *data;
    uint16_t count;

    Uniform(ProgramHandle handle, const char *name, void *ptr, UniformType type, uint16_t count)
        : handle(handle)
        , name(name)
        , data(nullptr)
        , count(count)
        , type(type) {
        size_t size = s_uniformTypeSize[type] * count;
        data = malloc(size);
        memcpy(data, ptr, size);
    }

    ~Uniform() {
        free(data);
    }

    Uniform(const Uniform& uniform)
        : handle(uniform.handle)
        , name(uniform.name)
        , data(nullptr)
        , count(uniform.count)
        , type(uniform.type) {
        size_t size = s_uniformTypeSize[uniform.type] * uniform.count;
        data = malloc(size);
        memcpy(data, uniform.data, size);
    }
};

} // namespace Bird