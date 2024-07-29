#pragma once

#include "TypeInfo.hpp"
#include "Coder.hpp"
#include "TypeName.hpp"

#include <unordered_map>
#include <string>
#include <utility>

namespace Rain {

class TypeRegistry;
class Codable;

TypeRegistry *getTypeRegistry();

template<typename T>
struct TypeEncoder;

template<typename T>
struct TypeDecoder;

template<typename T>
concept PrimitiveEncoder =
    requires(const char *key, Encoder *encoder, const TypeInfo &info, T &data) {
        TypeEncoder<T>::encode(key, encoder, info, data);
    };

template<typename T>
concept PrimitiveDecoder =
    requires(const char *key, Decoder *decoder, const TypeInfo &info, T &data) {
        TypeDecoder<T>::decode(key, decoder, info, data);
    };

class TypeRegistry {
private:
    template<typename T>
    static auto addFieldsIfHas(TypeInfo &) -> std::enable_if_t<!hasFields<T>> {}

    template<typename T>
    static auto addFieldsIfHas(TypeInfo &info) -> std::enable_if_t<hasFields<T>> {
        info.fields = StripType<T>::getFields();
    }

public:
    template<typename T>
    constexpr void fillCodingFunc(TypeInfo &info) {
        if constexpr (PrimitiveEncoder<T>) {
            info.encoderFunc =
                [](const char *key, Encoder *encoder, const TypeInfo &info, void *data) {
                    T *ptr = static_cast<T *>(data);
                    TypeEncoder<T>::encode(key, encoder, info, *ptr);
                };
        } else {
            info.encoderFunc =
                [](const char *key, Encoder *encoder, const TypeInfo &info, void *data) {
                    static_assert(std::is_base_of<Codable, T>());
                    encoder->beginObject(key);
                    for (auto &field : info.fields) {
                        auto memberInfo = getTypeRegistry()->findInfo(field.typeId);
                        memberInfo.encoderFunc(
                            field.name, encoder, memberInfo, addOffset(data, field.offset)
                        );
                    }
                    encoder->endObject();
                };
        }
        if constexpr (PrimitiveDecoder<T>) {
            info.decoderFunc =
                [](const char *key, Decoder *decoder, const TypeInfo &info, void *data) {
                    T *ptr = static_cast<T *>(data);
                    TypeDecoder<T>::decode(key, decoder, info, *ptr);
                };
        } else {
            info.decoderFunc =
                [](const char *key, Decoder *decoder, const TypeInfo &info, void *data) {
                    static_assert(std::is_base_of<Codable, T>());
                    decoder->beginObject(key);
                    for (auto &field : info.fields) {
                        auto memberInfo = getTypeRegistry()->findInfo(field.typeId);
                        memberInfo.decoderFunc(
                            field.name, decoder, memberInfo, addOffset(data, field.offset)
                        );
                    }
                    decoder->endObject();
                };
        }
    }

    template<typename T>
    TypeInfo makeTypeInfo(TypeId id, const std::string &name) {
        // std::cout << "CREATE TYPE INFO: " << name << std::endl;
        TypeInfo info(id, name);
        info.size = sizeof(T);
        info.align = alignof(T);
        fillCodingFunc<T>(info);
        return info;
    }

    template<typename T>
    TypeInfo findOrCreateType() {
        std::string_view name = typeName<T>();
        const TypeId hash = std::hash<std::string_view>()(name);
        const auto it = m_data.find(hash);
        if (it == m_data.end()) {
            std::string str(name);
            TypeInfo typeInfo = makeTypeInfo<T>(hash, str);
            addFieldsIfHas<T>(typeInfo);
            m_data.emplace(hash, typeInfo);
            return typeInfo;
        }
        TypeInfo a = it->second;
        return a;
    }

    TypeInfo &findInfo(TypeId id) {
        const auto it = m_data.find(id);
        // assert(it != data.end());
        return it->second;
    }

    template<typename FieldType>
    FieldInfo makeFieldInfo(const char *fieldName, uint32_t offset) {
        TypeInfo fieldTypeInfo = findOrCreateType<FieldType>();
        TypeId fieldTypeId = fieldTypeInfo.id;

        using TypeRemovedExtents = std::remove_all_extents_t<FieldType>;
        using TypeRemovedRefs = std::remove_reference_t<TypeRemovedExtents>;
        using TypeRemovedPtrs = RemoveAllPointers<TypeRemovedRefs>;
        constexpr bool isRef = std::is_reference_v<FieldType>;
        constexpr bool isRValRef = std::is_rvalue_reference_v<FieldType>;
        constexpr bool isConst = std::is_const_v<TypeRemovedPtrs>;
        constexpr bool isVolatile = std::is_volatile_v<TypeRemovedPtrs>;

        FieldInfo field(fieldTypeId, fieldName, offset, alignof(FieldType));

        if constexpr (isConst) {
            field.setConstFlag();
        }
        if constexpr (isVolatile) {
            field.setVolatileFlag();
        }
        if constexpr (isRef) {
            field.setReferenceFlag();
        }
        if constexpr (isRValRef) {
            field.setRValReferenceFlag();
        }

        return field;
    }

private:
    std::unordered_map<TypeId, TypeInfo> m_data;
};

// --------------------------------------------------------
// ---------- GENERAL RAIN CODING FUNCTION --------------
// --------------------------------------------------------

template<typename T>
void Encoder::encode(std::ostream &stream, T &data) {
    TypeInfo info = getTypeRegistry()->findOrCreateType<T>();
    encode(stream, &data, info);
}

template<typename T>
bool Decoder::decode(std::istream &stream, T &data) {
    TypeInfo info = getTypeRegistry()->findOrCreateType<T>();
    return decode(stream, &data, info);
}

// ------------------------------------------------------------
// ---------- PRIMITIVES DECODING IMPLEMENTATION --------------
// ------------------------------------------------------------

/** FLOAT */
template<>
struct TypeDecoder<float> {
    static void decode(const char *key, Decoder *decoder, const TypeInfo &info, float &data) {
        decoder->decode(key, data);
    }
};

/** CONST CHAR* */
template<>
struct TypeDecoder<const char *> {
    static void decode(const char *key, Decoder *decoder, const TypeInfo &info, const char *&data) {
        decoder->decode(key, data);
    }
};

/** STRING */
template<typename Elem, typename Traits, typename Alloc>
struct TypeDecoder<std::basic_string<Elem, Traits, Alloc>> {
    static void decode(
        const char *key,
        Decoder *decoder,
        const TypeInfo &info,
        std::basic_string<Elem, Traits, Alloc> &data
    ) {
        decoder->decode(key, data);
    }
};

/** INT */
template<>
struct TypeDecoder<int> {
    static void decode(const char *key, Decoder *decoder, const TypeInfo &info, int &data) {
        decoder->decode(key, data);
    }
};

/** VECTOR */
template<typename T, typename Alloc>
struct TypeDecoder<std::vector<T, Alloc>> {
    static void
    decode(const char *key, Decoder *decoder, const TypeInfo &info, std::vector<T, Alloc> &data) {
        decoder->beginArray(key);
        data.clear();
        auto memberInfo = getTypeRegistry()->findOrCreateType<T>();
        while (decoder->arrayHasElement()) {
            T value;
            memberInfo.decoderFunc(nullptr, decoder, memberInfo, &value);
            data.push_back(value);
            decoder->arrayNext();
        }
        decoder->endArray();
    }
};

// ------------------------------------------------------------
// ---------- PRIMITIVES ENCODING IMPLEMENTATION --------------
// ------------------------------------------------------------

/** FLOAT */
template<>
struct TypeEncoder<float> {
    static void encode(const char *key, Encoder *encoder, const TypeInfo &info, float &data) {
        encoder->encode(key, data);
    }
};

/** CONST CHAR* */
template<>
struct TypeEncoder<const char *> {
    static void encode(const char *key, Encoder *encoder, const TypeInfo &info, const char *&data) {
        encoder->encode(key, data);
    }
};

/** STRING */
template<typename Elem, typename Traits, typename Alloc>
struct TypeEncoder<std::basic_string<Elem, Traits, Alloc>> {
    static void encode(
        const char *key,
        Encoder *encoder,
        const TypeInfo &info,
        const std::basic_string<Elem, Traits, Alloc> &data
    ) {
        encoder->encode(key, data);
    }
};

/** INT */
template<>
struct TypeEncoder<int> {
    static void encode(const char *key, Encoder *encoder, const TypeInfo &info, int &data) {
        encoder->encode(key, data);
    }
};

/** VECTOR */
template<typename T, typename Alloc>
struct TypeEncoder<std::vector<T, Alloc>> {
    static void
    encode(const char *key, Encoder *encoder, const TypeInfo &info, std::vector<T, Alloc> &data) {
        encoder->beginArray(key);
        auto memberInfo = getTypeRegistry()->findOrCreateType<T>();
        for (auto &item : data) {
            memberInfo.encoderFunc(nullptr, encoder, memberInfo, &item);
        }
        encoder->endArray();
    }
};

} // namespace Rain
