#pragma once

#include "TypeRegistry.hpp"

// --------------------------------
// ---------- MACROS --------------
// --------------------------------

#define RAIN_FIELDS_BEGIN(classType)                                                               \
    static const std::vector<Rain::FieldInfo> getFields() {                                        \
        using ClassType = classType;                                                               \
        return std::vector<Rain::FieldInfo> ({

#define RAIN_FIELD(name)                                                                           \
    Rain::getTypeRegistry()->makeFieldInfo<decltype(name)>(#name, Rain::offsetOf(&ClassType::name)),

#define RAIN_FIELDS_END                                                                            \
    })                                                                                              \
    ;                                                                                              \
    }