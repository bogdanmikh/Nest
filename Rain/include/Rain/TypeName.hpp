#pragma once

#include <string_view>

namespace Rain {

template<typename T>
constexpr std::string_view typeName();

template<>
constexpr std::string_view typeName<void>() {
    return "void";
}

namespace TypeNameDetails {
    template<typename T>
    constexpr std::string_view wrappedTypeName() {
#ifdef __clang__
        return __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
        return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
        return __FUNCSIG__;
#else
#    error "Unsupported compiler"
#endif
    }

    constexpr std::size_t wrappedTypeNamePrefixLength() {
        return wrappedTypeName<void>().find(typeName<void>());
    }

    constexpr std::size_t wrappedTypeNameSuffixLength() {
        return wrappedTypeName<void>().length() - wrappedTypeNamePrefixLength() -
               typeName<void>().length();
    }
} // namespace TypeNameDetails

template<typename T>
constexpr std::string_view typeName() {
    using namespace TypeNameDetails;
    constexpr auto wrappedName = wrappedTypeName<T>();
    constexpr auto prefixLength = wrappedTypeNamePrefixLength();
    constexpr auto suffixLength = wrappedTypeNameSuffixLength();
    constexpr auto typeNameLength = wrappedName.length() - prefixLength - suffixLength;
    return wrappedName.substr(prefixLength, typeNameLength);
}

} // namespace Rain