#pragma once

#include "Nest/Renderer/Base.hpp"
#include <Nest/Logger/Logger.hpp>

struct Uniform {
    ProgramHandle handle;
    const char *name;
    void *value;
    UniformDataType type;
    uint32_t size;

    static uint32_t getSizeOfType(UniformDataType type) {
        switch (type) {
            case UniformDataType::Int:
                return sizeof(int);
            case UniformDataType::Mat4:
                return sizeof(float) * 16;
            case UniformDataType::IntArray:
                return sizeof(int) * 8;
        }
        LOG_ERROR("Uniform type is undefined");
        return 0;
    }

    Uniform(ProgramHandle handle, const char *name, void *value, UniformDataType type)
        : handle(handle)
        , name(name)
        , value(value)
        , type(type) {
        size = getSizeOfType(type);
    }

    Uniform()
        : handle(0)
        , name(nullptr)
        , value(nullptr)
        , type(UniformDataType::Int)
        , size(0) {}
};