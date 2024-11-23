//
// Created by Bogdan
//

#include "Bird/PlatformData.hpp"

namespace Bird {

PlatformData *PlatformData::get() {
    static PlatformData platformData;
    return &platformData;
}

} // namespace Bird