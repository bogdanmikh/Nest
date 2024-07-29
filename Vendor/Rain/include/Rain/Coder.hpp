#pragma once

#include "TypeInfo.hpp"

#include <istream>
#include <ostream>

namespace Rain {

class Encoder {
public:
    virtual ~Encoder() = default;

    template<typename T>
    void encode(std::ostream &stream, T &data);

    /// General encode
    virtual void encode(std::ostream &, void *data, TypeInfo info) = 0;
    /// Begin encoding object
    virtual void beginObject(const char *key) = 0;
    /// Encode int
    virtual void encode(const char *key, const int &data) = 0;
    /// Encode float
    virtual void encode(const char *key, const float &data) = 0;
    /// Encode string
    virtual void encode(const char *key, const std::string &data) = 0;
    /// Encode const char*
    virtual void encode(const char *key, const char *&data) = 0;
    /// End encoding object
    virtual void endObject() = 0;
    /// Begin encoding array
    virtual void beginArray(const char *key) = 0;
    /// End encoding array
    virtual void endArray() = 0;
};

class Decoder {
public:
    virtual ~Decoder() = default;

    template<typename T>
    bool decode(std::istream &stream, T &data);

    /// General decode
    virtual bool decode(std::istream &, void *data, TypeInfo info) = 0;
    /// Begin decoding object
    virtual bool beginObject(const char *key) = 0;
    /// Decode int
    virtual bool decode(const char *key, int &data) = 0;
    /// Decode float
    virtual bool decode(const char *key, float &data) = 0;
    /// Decode string
    virtual bool decode(const char *key, std::string &data) = 0;
    /// Decode const char*
    virtual bool decode(const char *key, const char *&data) = 0;
    /// End decoding object
    virtual void endObject() = 0;
    /// Begin decoding array
    virtual bool beginArray(const char *key) = 0;
    /// Check if array has current element
    virtual bool arrayHasElement() = 0;
    /// Go to next array element
    virtual void arrayNext() = 0;
    /// End decoding array
    virtual void endArray() = 0;
};

} // namespace Rain