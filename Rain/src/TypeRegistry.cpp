#include "Rain/TypeRegistry.hpp"

namespace Rain {

TypeRegistry *getTypeRegistry() {
    static TypeRegistry globalData;
    return &globalData;
}

} // namespace Rain