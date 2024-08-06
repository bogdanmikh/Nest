//
// Created by Admin on 18.02.2022.
//

#include "Bird/PlatformData.hpp"

namespace Bird {

PlatformData *PlatformData::get() {
    static PlatformData platformData;
    return &platformData;
}

} // namespace Bird