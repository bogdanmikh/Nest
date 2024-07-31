//
// Created by Admin on 18.02.2022.
//

#include "NestRen/PlatformData.hpp"

namespace NestRen {

PlatformData *PlatformData::get() {
    static PlatformData platformData;
    return &platformData;
}

} // namespace Miren