#include "Nest/Base/UUID.hpp"

#include <random>

static std::random_device s_randomDevice;
static std::mt19937 eng32(s_randomDevice());
static std::uniform_int_distribution<uint32_t> s_uniformDistribution32;

UUID::UUID()
    : m_UUID(s_uniformDistribution32(eng32)) {}

UUID::UUID(uint32_t uuid)
    : m_UUID(uuid) {}