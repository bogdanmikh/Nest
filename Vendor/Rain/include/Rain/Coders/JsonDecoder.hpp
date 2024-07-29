#pragma once

#include "Rain/Coder.hpp"

#include <vector>
#include <rapidjson/document.h>
#include <rapidjson/reader.h>
#include <rapidjson/istreamwrapper.h>

namespace Rain {

class JsonDecoder final : public Decoder {
    struct Object {
        const rapidjson::Value *ref;
        bool isPartOfArray;
    };

public:
    JsonDecoder();
    ~JsonDecoder() override = default;

    bool decode(std::istream &istream, void *data, TypeInfo info) override;
    bool beginObject(const char *key) override;
    bool decode(const char *key, int &data) override;
    bool decode(const char *key, float &data) override;
    bool decode(const char *key, std::string &data) override;
    bool decode(const char *key, const char *&data) override;
    void endObject() override;
    bool beginArray(const char *key) override;
    bool arrayHasElement() override;
    void arrayNext() override;
    void endArray() override;

private:
    const rapidjson::Value &currentObject();
    bool m_isArray;
    rapidjson::Document m_document;
    rapidjson::Document::AllocatorType &m_allocator;
    std::vector<Object> m_objectStack;
    std::vector<Object> m_arrayStack;
    std::vector<rapidjson::Value::ConstValueIterator> m_arrayIteratorStack;
};

} // namespace Rain