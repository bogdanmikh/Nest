#pragma once
// #pragma clang diagnostic push
// #pragma ide diagnostic ignored "google-explicit-constructor"

#include <cstdint>
#include <string>

class UUID final {
public:
    UUID();
    UUID(uint32_t uuid);

    operator uint32_t() {
        return m_UUID;
    }
    operator const uint32_t() const {
        return m_UUID;
    }

private:
    uint32_t m_UUID;
};

namespace std {

template<>
struct hash<UUID> {
    std::size_t operator()(const UUID &uuid) const {
        // uuid is already a randomly generated number, and is suitable as a hash key as-is.
        // this may change in the future, in which case return hash<uint64_t>{}(uuid); might be more
        // appropriate
        return uuid;
    }
};

} // namespace std