#pragma once

#include "Rain/Base.hpp"

#include <string>
#include <vector>

namespace Rain {

struct FieldInfo final {
    static constexpr uint32_t ConstFlag = 1 << 0; /**< Flag that marks if the variable is const*/
    static constexpr uint32_t ReferenceFlag =
        1 << 1; /**< Flag that marks if the variable is a reference (&)*/
    static constexpr uint32_t VolatileFlag =
        1 << 2; /**< Flag that marks if the variable is volatile*/
    static constexpr uint32_t RValReferenceFlag =
        1 << 3; /**< Flag that marks if the variable is a right value reference (&&)*/

    constexpr FieldInfo(TypeId id, const char *name, uint32_t offset, uint32_t align)
        : typeId(id)
        , name(name)
        , offset(offset)
        , align(align)
        , m_traitFlags(0)
        , m_pointerAmount(0) {}

    constexpr void setConstFlag() {
        m_traitFlags |= ConstFlag;
    }
    constexpr void setReferenceFlag() {
        m_traitFlags |= ReferenceFlag;
    }
    constexpr void setVolatileFlag() {
        m_traitFlags |= VolatileFlag;
    }
    constexpr void setRValReferenceFlag() {
        m_traitFlags |= RValReferenceFlag;
    }

    constexpr void setPointerAmount(uint16_t amount) {
        m_pointerAmount = amount;
    }
    constexpr uint32_t getPointerAmount() const {
        return m_pointerAmount;
    }

    constexpr bool isConst() const {
        return m_traitFlags & ConstFlag;
    }
    constexpr bool isReference() const {
        return m_traitFlags & ReferenceFlag;
    }
    constexpr bool isVolatile() const {
        return m_traitFlags & VolatileFlag;
    }
    constexpr bool isRValReference() const {
        return m_traitFlags & RValReferenceFlag;
    }
    constexpr bool isPointer() const {
        return m_pointerAmount;
    }
    constexpr bool isRefOrPointer() const {
        return isPointer() || isReference() || isRValReference();
    }

    TypeId typeId;
    const char *name;
    uint32_t offset;
    uint32_t align;

private:
    uint16_t m_pointerAmount;
    uint8_t m_traitFlags;
};

class Encoder;
class Decoder;

struct TypeInfo final {
    TypeId id;
    const std::string name;
    uint32_t size;
    uint32_t align;
    std::vector<FieldInfo> fields;
    void (*encoderFunc)(const char *key, Encoder *, const TypeInfo &info, void *data);
    void (*decoderFunc)(const char *key, Decoder *, const TypeInfo &info, void *data);

    TypeInfo(TypeId id, const std::string &name)
        : id(id)
        , name(name)
        , size(0)
        , align(0)
        , fields()
        , encoderFunc(nullptr)
        , decoderFunc(nullptr) {}
};

} // namespace Rain
