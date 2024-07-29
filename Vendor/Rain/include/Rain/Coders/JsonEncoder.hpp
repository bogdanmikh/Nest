#pragma once

#include "Rain/Coder.hpp"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>

namespace Rain {

class JsonEncoder final : public Encoder {
    struct Object {
        rapidjson::Value *ref;
        bool isPartOfArray;
    };

public:
    explicit JsonEncoder(bool pretty);
    ~JsonEncoder() override = default;

    void encode(std::ostream &, void *data, TypeInfo info) override;
    void beginObject(const char *key) override;
    void encode(const char *key, const int &data) override;
    void encode(const char *key, const float &data) override;
    void encode(const char *key, const std::string &data) override;
    void encode(const char *key, const char *&data) override;
    void endObject() override;
    void beginArray(const char *key) override;
    void endArray() override;

private:
    bool m_pretty;
    bool m_isArray;
    rapidjson::Document m_document;
    rapidjson::Document::AllocatorType *m_allocator;

    std::vector<Object> m_objectStack;
    std::vector<Object> m_arrayStack;

    rapidjson::Value &currentObject();
    void addValue(const char *key, rapidjson::Value &value);
};

} // namespace Rain
